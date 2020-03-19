//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GL_BACKEND_HPP
#define XE_GL_BACKEND_HPP

#include <xe/memory.hpp>
#include <xe/params.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/display_list.hpp>

namespace xe::gpu {

  class Backend : public Object {
  XE_OBJECT(Backend, Object);
  public:
    struct Uniform {
      int32_t loc = -1;
      std::string name;
      VertexFormat::Enum type = VertexFormat::Undefined;
      uint32_t size = 0;
      uint32_t offset = 0;
    };

    struct Buffer {
      uint32_t buffer = 0;
    };

    struct Texture {
      uint32_t texture = 0;
      uint32_t format = 0;
      uint32_t internalFormat = 0;
      uint32_t type = 0;
      uint32_t target = 0;
    };

    struct Pipeline {
      int32_t program = 0;
      int32_t textureUniformsLoc[cMaxTextureUnits] = { };
      Uniform uniforms[cMaxShaderUniforms] = { };
      memory<uint8_t> uniformData;
      uint32_t usedUniforms = 0;
    };

    struct Framebuffer {
      uint32_t framebuffer = 0;
    };

  public:
    memory<Buffer> buffers;
    memory<Texture> textures;
    memory<Pipeline> pipelines;
    memory<Framebuffer> framebuffers;

    static std::atomic<uint32_t> drawCalls;

  public:
    static void initBackend(Backend **b, const Params::GPU &params);
    static void destroyBackend(Backend **b);

    static void clear(const DisplayList::ClearData &d);
    static void setupView(DisplayList::ViewData &d);
    static void fillBuffer(DisplayList::FillBufferData &d);
    static void fillTexture(DisplayList::FillTextureData &d);
    static void setupPipeline(DisplayList::SetupPipelineData &d);
    static void render(DisplayList::RenderData &d);
    static void destroy(DisplayList::DestroyData &d);

  private:
    static void destroyResource(RenderContext *ctx, Resource::ResourceType type, uint32_t pos);
  };

}

#endif //XE_GL_BACKEND_HPP
