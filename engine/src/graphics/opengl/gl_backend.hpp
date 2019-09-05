//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GL_BACKEND_HPP
#define XE_GL_BACKEND_HPP

#include <xe/scoped_array.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/draw_list.hpp>

namespace xe::gpu {

  struct BackEnd : public Object {
  XE_OBJECT(BackEnd, Object);

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

    scoped_array<Buffer> buffers;
    scoped_array<Texture> textures;
    scoped_array<Material> materials;
    scoped_array<Framebuffer> framebuffers;
  };

  void initBackEnd(BackEnd **backEnd, const Params::GPU &params);
  void destroyBackEnd(BackEnd **backEnd);

  void clear(const DrawList::ClearData &d);
  void fillBuffer(DrawList::FillBufferData &d);
  void fillTexture(DrawList::FillTextureData &d);
  void setupMaterial(DrawList::SetupMaterialData &d);
  void render(DrawList::RenderData &d);
}

#endif //XE_GL_BACKEND_HPP
