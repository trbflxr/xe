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

  static constexpr const char *vfsBasePath = "/textures/";

  void *Texture::loadFromFile(const char *file, Texture::Info &tex, bool flip) {
    XE_TRACE_BEGIN("XE", "Texture load");

    std::string path(file);

    if (!FileSystem::exists(path)) {
      path.insert(0, vfsBasePath);
    }

    XE_CORE_TRACE("Loading: ", path);

    int64 memorySize;
    byte *memory = VFS::readFile(path, &memorySize);

    if (!memory) {
      XE_CORE_ERROR("[TEXTURE / Loader] Texture load failed. Unable to load image ({})", path);
      XE_TRACE_END("XE", "Texture load");
      return nullptr;
    }

    int32 width = 0;
    int32 height = 0;
    int32 channels = 0;
    int32 bpp = 0;
    void *data = nullptr;
    bool hdr = false;

    std::string extension = string::getFileExt(file);

    if (extension == "hdr") {
      data = stbi_loadf_from_memory(memory, static_cast<int32>(memorySize), &width, &height, &channels, 0);
      hdr = true;
      bpp = channels * sizeof(float);
    } else {
      data = stbi_load_from_memory(memory, static_cast<int32>(memorySize), &width, &height, &channels, 0);
      bpp = channels * sizeof(byte);
    }

    delete[] memory;

    if (flip) {
      stbi__vertical_flip(data, width, height, bpp);
    }

    if (!data) {
      XE_CORE_ERROR("[TEXTURE / Loader] Texture load failed. Unsupported format ({})", path);
      XE_TRACE_END("XE", "Texture load");
      return nullptr;
    }

    if (width < 1 || height < 1) {
      stbi_image_free(data);
      XE_CORE_ERROR("[TEXTURE / Loader] Texture load failed. Invalid texture data ({})", path);
      XE_TRACE_END("XE", "Texture load");
      return nullptr;
    }

    tex.width = width;
    tex.height = height;

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
        XE_CORE_ERROR("[TEXTURE / Loader] Texture load failed. Invalid texture channels ({})", path);
        XE_TRACE_END("XE", "Texture load");
        return nullptr;
      }
    }

    XE_CORE_INFO("[TEXTURE / Loader] Texture loaded ({})", path);

    XE_TRACE_END("XE", "Texture load");
    return data;
  }

  std::vector<void *> Texture::loadCubemapFromFile(std::string_view rt, std::string_view lf,
                                                   std::string_view up, std::string_view dn,
                                                   std::string_view bk, std::string_view ft,
                                                   Texture::Info &tex, bool flip) {

    XE_ASSERT(false, "Not supported yet");
    return std::vector<void *>();
  }

  Texture Framebuffer::colorAttachment(uint16 index) const {
    if (!ctx) {
      return Texture();
    }
    RenderContext::checkValidResource(id, &ctx->framebuffers_);
    const uint loc = RenderContext::index(id);
    return ctx->framebuffers_[loc].colorAttachments[index];
  }

  Texture Framebuffer::depthStencilAttachment() const {
    if (!ctx) {
      return Texture();
    }
    RenderContext::checkValidResource(id, &ctx->framebuffers_);
    const uint loc = RenderContext::index(id);
    return ctx->framebuffers_[loc].depthAttachment;
  }

  void Framebuffer::setColorAttachment(Texture t, uint16 index) {
    if (!ctx) {
      return;
    }
    RenderContext::checkValidResource(id, &ctx->framebuffers_);
    const uint loc = RenderContext::index(id);
    ctx->framebuffers_[loc].colorAttachments[index] = t;
  }

  void Framebuffer::setDepthStencilAttachment(Texture t) {
    if (!ctx) {
      return;
    }
    RenderContext::checkValidResource(id, &ctx->framebuffers_);
    const uint loc = RenderContext::index(id);
    ctx->framebuffers_[loc].depthAttachment = t;
  }

}

