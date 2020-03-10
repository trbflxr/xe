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

  uint32_t RenderContext::index(uint32_t id) {
    return id & 0x000fffff;
  }

  std::pair<uint32_t, uint32_t> RenderContext::indexAndVersion(uint32_t id) {
    const uint32_t pos = id & 0x000fffff;
    const uint32_t version = (id & 0xfff00000) >> 20;
    return {pos, version};
  }

  size_t RenderContext::computeSize(VertexFormat::Enum type) {
    switch (type) {
      case VertexFormat::Int8:
      case VertexFormat::Uint8: return 1;

      case VertexFormat::Int16:
      case VertexFormat::Uint16: return 2;

      case VertexFormat::Int32:
      case VertexFormat::Uint32:
      case VertexFormat::Float: return 4;

      case VertexFormat::Float2: return 8;
      case VertexFormat::Float3: return 12;
      case VertexFormat::Float4: return 16;
      case VertexFormat::Mat4: return 64;

      default: return 0;
    }
  }

}

