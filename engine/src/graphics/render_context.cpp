//
// Created by FLXR on 9/4/2019.
//

#include "xepch.hpp"
#ifdef XE_PLATFORM_GL
  #include "graphics/opengl/gl_backend.hpp"
#else
  #error only gl currently supported
#endif
#include <xe/graphics/render_context.hpp>

namespace xe {

  RenderContext::RenderContext() {

  }

  RenderContext::~RenderContext() {
    gpu::destroyBackEnd(&backEnd_);
  }

  void RenderContext::init(const Params::GPU &params) {
    gpu::initBackEnd(&backEnd_, params);

    buffers_.alloc(params.maxBuffers);
    textures_.alloc(params.maxTextures);
    materials_.alloc(params.maxMaterials);

    XE_CORE_INFO("[GPU / RenderContext] Initialized RenderContext with params:\n"
                 "\t- maxBuffers\t\t({})\n"
                 "\t- maxTextures\t\t({})\n"
                 "\t- maxMaterials\t\t({})\n"
                 "\t- maxFramebuffers\t({})",
                 params.maxBuffers, params.maxTextures, params.maxMaterials, params.maxFramebuffers);
  }

  uint RenderContext::index(uint id) {
    return id & 0x000FFFFF;
  }

  std::pair<uint, uint> RenderContext::indexAndVersion(uint id) {
    const uint pos = id & 0x000FFFFF;
    const uint version = (id & 0xFFF00000) >> 20;
    return {pos, version};
  }

}

