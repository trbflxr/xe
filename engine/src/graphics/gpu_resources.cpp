//
// Created by FLXR on 9/5/2019.
//

#include "xepch.hpp"
#include <xe/graphics/gpu_resources.hpp>

#include <xe/graphics/render_context.hpp>
#include <xe/core/vfs.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
  #define STB_IMAGE_IMPLEMENTATION
#endif
#include "external/stb/stb_image.h"

namespace xe::gpu {

  void *Texture::loadFromFile(std::string_view file, Texture::Info &tex, bool flip) {
#if XE_DEBUG_TRACING
    const std::string name = std::string("Texture load file: ") + file.data();
#else
    static const std::string name("Texture load");
#endif
    XE_TRACE_BEGIN("XE", name.c_str());
    XE_CORE_TRACE("[Texture / Loader] Loading: '{}'", file);

    auto memory = VFS::readBytes(file);
    if (memory.empty()) {
      XE_CORE_ERROR("[Texture / Loader] Texture load failed. Unable to load image '{}'", file);
      XE_TRACE_END("XE", name.c_str());
      return nullptr;
    }

    int32_t width = 0;
    int32_t height = 0;
    int32_t channels = 0;
    int32_t bpp = 0;
    void *data = nullptr;
    bool hdr = false;

    std::string extension = string::getFileExt(file);

    if (extension == "hdr") {
      data = stbi_loadf_from_memory(memory.data(), static_cast<int32_t>(memory.size()), &width, &height, &channels, 0);

      hdr = true;
      bpp = channels * sizeof(float);
    } else {
      data = stbi_load_from_memory(memory.data(), static_cast<int32_t>(memory.size()), &width, &height, &channels, 0);
      bpp = channels * sizeof(uint8_t);
    }

    if (flip) {
      stbi__vertical_flip(data, width, height, bpp);
    }

    if (!data) {
      XE_CORE_ERROR("[Texture / Loader] Texture load failed. Unsupported format '{}'", file);
      XE_TRACE_END("XE", name.c_str());
      return nullptr;
    }

    if (width < 1 || height < 1) {
      stbi_image_free(data);

      XE_CORE_ERROR("[Texture / Loader] Texture load failed. Invalid texture data '{}'", file);
      XE_TRACE_END("XE", name.c_str());
      return nullptr;
    }

    tex.width = static_cast<uint32_t>(width);
    tex.height = static_cast<uint32_t>(height);

    switch (bpp) {
      case 1: {
        tex.format = hdr ? TexelsFormat::R16f : TexelsFormat::R8;
        break;
      }
      case 2: {
        tex.format = hdr ? TexelsFormat::Rg16f : TexelsFormat::Rg8;
        break;
      }
      case 3: tex.format = hdr ? TexelsFormat::Rgb16f : TexelsFormat::Rgb8;
        break;
      case 4: {
        tex.format = hdr ? TexelsFormat::Rgba16f : TexelsFormat::Rgba8;
        break;
      }
      default: {
        XE_CORE_ERROR("[Texture / Loader] Texture load failed. Invalid texture channels '{}'", file);
        XE_TRACE_END("XE", name.c_str());
        return nullptr;
      }
    }

    XE_CORE_INFO("[Texture / Loader] Texture loaded '{}'", file);

    XE_TRACE_END("XE", name.c_str());
    return data;
  }

  std::vector<void *> Texture::loadCubemapFromFile(std::string_view /*rt*/, std::string_view /*lf*/,
                                                   std::string_view /*up*/, std::string_view /*dn*/,
                                                   std::string_view /*bk*/, std::string_view /*ft*/,
                                                   Texture::Info &/*tex*/, bool /*flip*/) {

    XE_ASSERT(false, "Not supported yet");
    return std::vector<void *>();
  }

  std::string Pipeline::makeShader(const std::initializer_list<std::string_view> &sources) {
    //todo: get version from somewhere
    static const char *version = "#version 460 core\n";
    std::string shader = version;

    for (const auto &s : sources) {
      shader += s;
    }

    return shader;
  }

  Texture Framebuffer::colorAttachment(uint32_t index) const {
    if (!ctx) {
      return Texture();
    }
    RenderContext::checkValidResource(id, &ctx->framebuffers_);
    const uint32_t loc = RenderContext::index(id);
    return *ctx->framebuffers_[loc].colorAttachments[index];
  }

  Texture Framebuffer::depthStencilAttachment() const {
    if (!ctx) {
      return Texture();
    }
    RenderContext::checkValidResource(id, &ctx->framebuffers_);
    const uint32_t loc = RenderContext::index(id);
    return *ctx->framebuffers_[loc].depthAttachment;
  }

  void Framebuffer::setColorAttachment(std::shared_ptr<Texture> &&t, uint32_t index) {
    if (!ctx) {
      return;
    }
    RenderContext::checkValidResource(id, &ctx->framebuffers_);
    const uint32_t loc = RenderContext::index(id);
    ctx->framebuffers_[loc].colorAttachments[index] = t;
  }

  void Framebuffer::setDepthStencilAttachment(std::shared_ptr<Texture> &&t) {
    if (!ctx) {
      return;
    }
    RenderContext::checkValidResource(id, &ctx->framebuffers_);
    const uint32_t loc = RenderContext::index(id);
    ctx->framebuffers_[loc].depthAttachment = t;
  }

}

