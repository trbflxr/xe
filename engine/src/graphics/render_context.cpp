//
// Created by FLXR on 9/4/2019.
//

#include "xepch.hpp"
#include <xe/graphics/render_context.hpp>
#ifdef XE_PLATFORM_GL
  #include "graphics/opengl/gl_backend.hpp"
#else
  #error only gl currently supported
#endif

namespace xe {

  RenderContext::RenderContext() {

  }

  RenderContext::~RenderContext() {
    gpu::Backend::destroyBackend(&backend_);
  }

  void RenderContext::init(const Params::GPU &params) {
    gpu::Backend::initBackend(&backend_, params);

    buffers_.alloc(params.maxBuffers);
    textures_.alloc(params.maxTextures);
    pipelines_.alloc(params.maxPipelines);
    framebuffers_.alloc(params.maxFramebuffers);

    XE_CORE_INFO("[GPU / RenderContext] Initialized RenderContext with params:\n"
                 "\t- maxBuffers\t\t({})\n"
                 "\t- maxTextures\t\t({})\n"
                 "\t- maxPipelines\t\t({})\n"
                 "\t- maxFramebuffers\t({})",
                 params.maxBuffers, params.maxTextures, params.maxPipelines, params.maxFramebuffers);
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

