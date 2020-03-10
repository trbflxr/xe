//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GL_BACKEND_HPP
#define XE_GL_BACKEND_HPP

#include <xe/memory.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/display_list.hpp>

namespace xe::gpu {

  class Backend : public Object {
  XE_OBJECT(Backend, Object);
  public:
    struct Uniform {
      int32 loc = -1;
      std::string name;
      VertexFormat::Enum type = VertexFormat::Undefined;
      uint size = 0;
      uint offset = 0;
    };

    struct Buffer {
      uint buffer = 0;
    };

    struct Texture {
      uint texture = 0;
      uint format = 0;
      uint internalFormat = 0;
      uint type = 0;
      uint target = 0;
    };

    struct Pipeline {
      int32 program = 0;
      int32 textureUniformsLoc[cMaxTextureUnits] = { };
      Uniform uniforms[cMaxShaderUniforms] = { };
      memory<byte> uniformData;
      size_t usedUniforms = 0;
    };

    struct Framebuffer {
      uint framebuffer = 0;
    };

  public:
    static void initBackend(Backend **b, const Params::GPU &params);
    static void destroyBackend(Backend **b);

    static void clear(const DisplayList::ClearData &d);
    static void setupView(DisplayList::ViewData &d);
    static void fillBuffer(DisplayList::FillBufferData &d);
    static void fillTexture(DisplayList::FillTextureData &d);
    static void setupPipeline(DisplayList::SetupPipelineData &d);
    static void render(DisplayList::RenderData &d);

  private:
    memory<Buffer> buffers_;
    memory<Texture> textures_;
    memory<Pipeline> pipelines_;
    memory<Framebuffer> framebuffers_;
  };

}

#endif //XE_GL_BACKEND_HPP
