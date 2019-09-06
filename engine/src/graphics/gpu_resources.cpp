//
// Created by FLXR on 9/5/2019.
//

#include "xepch.hpp"
#include <xe/graphics/gpu_resources.hpp>
#include <xe/graphics/render_context.hpp>
#ifndef STB_IMAGE_IMPLEMENTATION
  #define STB_IMAGE_IMPLEMENTATION
#endif
#include "external/stb/stb_image.h"

namespace xe::gpu {

  void *Texture::loadFromFile(const char *file, Texture::Info &tex, bool flip) {
    //todo: handle file not found
    //todo: vfs support

    XE_TRACE_BEGIN("XE", "Texture load");

    int32 width = 0;
    int32 height = 0;
    int32 channels = 0;
    int32 bpp = 0;
    void *data = nullptr;
    bool hdr = false;

    string extension = string::getFileExt(file);

    if (extension == "hdr") {
      data = stbi_loadf(file, &width, &height, &channels, 0);
      hdr = true;
      bpp = channels * sizeof(float);
    } else {
      data = stbi_load(file, &width, &height, &channels, 0);
      bpp = channels * sizeof(byte);
    }

    if (flip) {
      stbi__vertical_flip(data, width, height, bpp);
    }

    if (!data) {
      XE_CORE_ERROR("[TEXTURE / Loader] Texture load failed. Unsupported format");
      return nullptr;
    }

    if (width < 1 || height < 1) {
      stbi_image_free(data);
      XE_CORE_ERROR("[TEXTURE / Loader] Texture load failed. Invalid texture data");
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
        XE_CORE_ERROR("[TEXTURE / Loader] Texture load failed. Invalid texture channels");
        return nullptr;
      }
    }


    XE_CORE_INFO("[TEXTURE / Loader] Texture loaded ({})", file);

    XE_TRACE_END("XE", "Texture load");
    return data;
  }

  vector<void *> Texture::loadCubemapFromFile(const string &rt, const string &lf,
                                              const string &up, const string &dn,
                                              const string &bk, const string &ft,
                                              Texture::Info &tex, bool flip) {

    assert(false);
    return xe::vector<void *>();
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

