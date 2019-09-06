//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GL_BACKEND_HPP
#define XE_GL_BACKEND_HPP

#include <xe/memory.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/display_list.hpp>

namespace xe::gpu {

  class XE_API Backend : public Object {
  XE_OBJECT(Backend, Object);
  public:
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
      uint program = 0;
      int32 textureUniformsLoc[cMaxTextureUnits] = { };
    };

    struct Framebuffer {
      uint framebuffer = 0;
    };

  public:
    static void initBackEnd(Backend **backEnd, const Params::GPU &params);
    static void destroyBackEnd(Backend **backEnd);

    static void clear(const DisplayList::ClearData &d);
    static void setupView(DisplayList::ViewData &d);
    static void fillBuffer(DisplayList::FillBufferData &d);
    static void fillTexture(DisplayList::FillTextureData &d);
    static void setupPipeline(DisplayList::SetupPipelineData &d);
    static void render(DisplayList::RenderData &d);

  public:
    memory<Buffer> buffers;
    memory<Texture> textures;
    memory<Pipeline> pipelines;
    memory<Framebuffer> framebuffers;
  };
}

#endif //XE_GL_BACKEND_HPP
