//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GL_BACKEND_HPP
#define XE_GL_BACKEND_HPP

#include <xe/scoped_array.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/draw_list.hpp>

namespace xe::gpu {

  class XE_API BackEnd : public Object {
  XE_OBJECT(BackEnd, Object);
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

    struct Material {
      uint program = 0;
      int32 textureUniformsLoc[cMaxTextureUnits] = { };
    };

    struct Framebuffer {
      uint framebuffer = 0;
    };

  public:
    static void initBackEnd(BackEnd **backEnd, const Params::GPU &params);
    static void destroyBackEnd(BackEnd **backEnd);

    static void clear(const DrawList::ClearData &d);
    static void setupView(DrawList::ViewData &d);
    static void fillBuffer(DrawList::FillBufferData &d);
    static void fillTexture(DrawList::FillTextureData &d);
    static void setupMaterial(DrawList::SetupMaterialData &d);
    static void render(DrawList::RenderData &d);

  public:
    scoped_array<Buffer> buffers;
    scoped_array<Texture> textures;
    scoped_array<Material> materials;
    scoped_array<Framebuffer> framebuffers;
  };
}

#endif //XE_GL_BACKEND_HPP
