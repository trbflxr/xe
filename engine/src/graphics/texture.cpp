//
// Created by trbflxr on 3/15/2020.
//

#include "xepch.hpp"
#include <xe/graphics/texture.hpp>

#ifdef XE_PLATFORM_GL
  #include "graphics/opengl/gl_backend.hpp"
#else
  #error only gl currently supported
#endif

#include <xe/graphics/render_context.hpp>
#include <xe/core/engine.hpp>
#include <xe/assert.hpp>

namespace xe {

  Texture::Texture() {
    setName("Texture");
  }

  Texture::Texture(const gpu::Texture::Info &info) {
    setName("Texture");
    gpu_.info = info;
  }

  Texture::Texture(std::string_view file, gpu::Texture::Info info) {
    setName("Texture");
    gpu_.info = info;

    for (auto &d : data_) {
      d = nullptr;
    }

    if (gpu_.info.type == TextureType::CubeMap) {
      XE_ASSERT(false, "Unsupported");
      //todo: implement
      //gpu::Texture::loadCubemapFromFile();
    } else {
      data_[0] = gpu::Texture::loadFromFile(file, gpu_.info);
    }
  }

  Texture::~Texture() {
    for (auto &data : data_) {
      if (data) {
        if (hdr_) {
          delete reinterpret_cast<float *>(data);
        } else {
          delete reinterpret_cast<uint8_t *>(data);
        }
        data = nullptr;
      }
    }
  }

  bool Texture::setup() {
    if (loading_) {
      return false;
    }

    XE_TRACE_BEGIN("XE", "Texture Setup");
    gpu_.info.bindless = true;
    DisplayList commands;
    if (!gpu_.tex || !gpu_.tex->id) {
      gpu_.tex = Engine::ref().gpu().createTexture(gpu_.info);
      if (!gpu_.tex) {
        XE_CORE_CRITICAL("[Texture] Failed to create texture");
        return false;
      }
    }
    commands.fillTextureCommand()
        .set_texture(*gpu_.tex)
        .set_data0(data_[0])
        .set_data1(data_[1])
        .set_data2(data_[2])
        .set_data3(data_[3])
        .set_data4(data_[4])
        .set_data5(data_[5])
        .set_offsetX(gpu_.offset[0])
        .set_offsetY(gpu_.offset[1])
        .set_offsetZ(gpu_.offset[2])
        .set_buildMipmap(gpu_.buildMipmap)
        .set_width(gpu_.info.width)
        .set_height(gpu_.info.height)
        .set_depth(gpu_.info.depth);
    Engine::ref().executeOnGpu(std::move(commands));
    dirty_ = false;

    XE_TRACE_END("XE", "Texture Setup");
    return true;
  }

  void Texture::destroy() {
    if (gpu_.tex) {
      Engine::ref().gpu().destroyResource(*gpu_.tex);
    }
  }

  bool Texture::loadFromFile(std::string_view file) {
    file_ = file;
    data_[0] = gpu::Texture::loadFromFile(file, gpu_.info);
    return data_[0];
  }

  void Texture::setInfo(const gpu::Texture::Info &info) {
    gpu_.info = info;
    dirty_ = true;
  }

  void Texture::setSize(uint32_t width, uint32_t height, uint32_t depth) {
    gpu_.info.width = width;
    gpu_.info.height = height;
    gpu_.info.depth = depth;
    dirty_ = true;
  }

  void Texture::setOffset(uint32_t x, uint32_t y, uint32_t z) {
    gpu_.offset[0] = x;
    gpu_.offset[1] = y;
    gpu_.offset[2] = z;
    dirty_ = true;
  }

  void Texture::setFiltering(TextureMinFilter min, TextureMagFilter mag) {
    gpu_.info.minFilter = min;
    gpu_.info.magFilter = mag;
    dirty_ = true;
  }

  void Texture::setWrapping(TextureWrap wrap) {
    setWrapping(wrap, wrap, wrap);
    dirty_ = true;
  }

  void Texture::setWrapping(TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR) {
    gpu_.info.wrapS = wrapS;
    gpu_.info.wrapT = wrapT;
    gpu_.info.wrapR = wrapR;
    dirty_ = true;
  }

  void Texture::setTexelsFormat(TexelsFormat format) {
    gpu_.info.format = format;
    dirty_ = true;
  }

  void Texture::setUsage(Usage usage) {
    gpu_.info.usage = usage;
    dirty_ = true;
  }

  void Texture::setType(TextureType type) {
    gpu_.info.type = type;
    dirty_ = true;
  }

  void Texture::setBuildMipmap(bool buildMipmap) {
    gpu_.buildMipmap = buildMipmap;
    dirty_ = true;
  }

  void Texture::setData(void *data, uint32_t index) {
    if (data_[index]) {
      if (hdr_) {
        delete reinterpret_cast<float *>(data_[index]);
      } else {
        delete reinterpret_cast<uint8_t *>(data_[index]);
      }
      data_[index] = nullptr;
    }
    data_[index] = data;
  }

  void Texture::setData(Color color, uint32_t index) {
    if (data_[index] != nullptr) {
      if (hdr_) {
        delete reinterpret_cast<float *>(data_[index]);
      } else {
        delete reinterpret_cast<uint8_t *>(data_[index]);
      }
      data_[index] = nullptr;
    }

    data_[index] = new uint8_t[3];

    static uint8_t backgroundColor[] = {
        static_cast<uint8_t>(color.r * 255.0f),
        static_cast<uint8_t>(color.g * 255.0f),
        static_cast<uint8_t>(color.b * 255.0f)
    };

    dirty_ = true;
    hdr_ = false;

    gpu_.info.width = 1;
    gpu_.info.height = 1;
    gpu_.info.minFilter = TextureMinFilter::Nearest;
    gpu_.info.magFilter = TextureMagFilter::Nearest;
    gpu_.info.wrapS = TextureWrap::Repeat;
    gpu_.info.wrapT = TextureWrap::Repeat;
    gpu_.info.wrapR = TextureWrap::Repeat;
    gpu_.info.type = TextureType::T2D;
    gpu_.info.format = TexelsFormat::Rgb8;
    std::memcpy(data_[index], backgroundColor, 3);
  }

  void Texture::setHdr(bool hdr) {
    hdr_ = hdr;
  }

  void *Texture::data(uint32_t index) const {
    XE_ASSERT(index < 6, "Index out of range");
    return data_[index];
  }

  uint64_t Texture::handle() {
    if (!handle_) {
      if (!gpu_.tex->ctx) {
        return 0;
      }
      auto b = RenderContext::getResource(gpu_.tex->id, &gpu_.tex->ctx->textures_, &gpu_.tex->ctx->backend_->textures);
      handle_ = b.first->handle;
    }
    return handle_;
  }

}
