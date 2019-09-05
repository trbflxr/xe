//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_DRAW_LIST_HPP
#define XE_DRAW_LIST_HPP

#include <xe/memory.hpp>
#include <xe/vector.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/color.hpp>
#include <xe/graphics/gpu_resources.hpp>

#pragma warning(disable : 4114) //same type qualifier used more than once

namespace xe {

  class XE_API DrawList : public Object {
  XE_OBJECT(DrawList, Object);
    friend class GPU;
  public:
    struct Command;

    explicit DrawList();

    void update();

#define PROPERTY(type, name, ...) \
  type name = __VA_ARGS__;\
  Self& set_##name(const type &c) { name = c; return *this; }

#define PROPERTY_PTR(type, name) \
  const type *name = nullptr;\
  Self& set_##name(const type *c) { name = c; return *this; }

#define PROP_ARRAY(type, count, name) \
  type name[count] = {};\
  Self& set_##name(size_t i, const type &c) { name[i] = c; return *this; }\
  Self& set_v_##name(std::vector<type> c) { for (uint32 i = 0; i < c.size(); ++i) { set_##name(i, c[i]); } return *this; }

    struct ClearData {
      typedef ClearData Self;
      PROPERTY(Color, color, { 0.0f, 0.0f, 0.0f, 1.0f });
      PROPERTY(float, depth, 1.0f);
      PROPERTY(int32, stencil, 0);
      PROPERTY(bool, clearColor, true);
      PROPERTY(bool, clearDepth, true);
      PROPERTY(bool, clearStencil, false);
    };

    struct FillBufferData {
      typedef FillBufferData Self;
      PROPERTY(const char*, name, "");
      PROPERTY(gpu::Buffer, buffer, { });
      PROPERTY(uint, offset, 0);
      PROPERTY(uint, size, 0);
      PROPERTY_PTR(void, data);
    };

    struct FillTextureData {
      typedef FillTextureData Self;
      PROPERTY(gpu::Texture, texture, { });
      PROPERTY(uint16, offsetX, 0);
      PROPERTY(uint16, offsetY, 0);
      PROPERTY(uint16, offsetZ, 0);
      PROPERTY(uint16, width, 0);
      PROPERTY(uint16, height, 0);
      PROPERTY(uint16, depth, 0);
      PROPERTY(bool, buildMipmap, false);
      PROPERTY_PTR(void, data0);
      PROPERTY_PTR(void, data1);
      PROPERTY_PTR(void, data2);
      PROPERTY_PTR(void, data3);
      PROPERTY_PTR(void, data4);
      PROPERTY_PTR(void, data5);
    };

    struct SetupMaterialData {
      typedef SetupMaterialData Self;
      PROPERTY(gpu::Material, material, { });
      PROP_ARRAY(gpu::Texture, cMaxTextureUnits, texture);
      PROP_ARRAY(gpu::Buffer, cMaxVertexAttribs, buffer);
      PROP_ARRAY(gpu::Buffer, cMaxUniformBuffers + 1, uniformBuffer);
      PROPERTY(vec4, scissor, vec4(0.0f));
      PROPERTY(mat4, modelMatrix, mat4());
    };

    struct RenderData {
      typedef RenderData Self;
      PROPERTY(gpu::Buffer, indexBuffer, { });
      PROPERTY(uint32, offset, 0);
      PROPERTY(uint32, count, 0);
      PROPERTY(uint32, instances, 1);
      PROPERTY(IndexFormat, type, IndexFormat::Uint16);
    };

#undef PROPERTY
#undef PROPERTY_PTR
#undef PROP_ARRAY

    ClearData &clearCommand();
    FillBufferData &fillBufferCommand();
    FillTextureData &fillTextureCommand();
    SetupMaterialData &setupMaterialCommand();
    RenderData &renderCommand();

  private:
    vector<ref_ptr<Command>> commands_;
  };

}

#endif //XE_DRAW_LIST_HPP
