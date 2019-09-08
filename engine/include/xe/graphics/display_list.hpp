//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_DISPLAY_LIST_HPP
#define XE_DISPLAY_LIST_HPP

#include <xe/memory.hpp>
#include <xe/vector.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/color.hpp>
#include <xe/graphics/gpu_resources.hpp>

#pragma warning(disable : 4114) //same type qualifier used more than once

namespace xe {

  struct Uniform {
    const char *name = nullptr;
    byte data[64] = { };
    uint count = 1;

    Uniform() : name(nullptr), count(1) { }

    Uniform(const char *name, void *data, size_t size, uint count = 1) :
        name(name),
        count(count) {

      memcpy(Uniform::data, data, size);
    }
  };

  class XE_API DisplayList : public Object {
  XE_OBJECT(DisplayList, Object);
    friend class GPU;
  public:
    struct Command;

    explicit DisplayList();

    void update();

#define PROP(type, name, ...) \
    type name = __VA_ARGS__;\
    Self &set_##name(const type &c) { name = c; return *this; }

#define PROP_PTR(type, name) \
    const type *name = nullptr;\
    Self &set_##name(const type *c) { name = c; return *this; }

#define PROP_ARRAY(type, count, name) \
    type name[count] = {};\
    Self &set_##name(size_t i, const type &c) { name[i] = c; return *this; }\
    Self &set_v_##name(const vector<type> &c) { for (uint i = 0; i < c.size(); ++i) { set_##name(i, c[i]); } return *this; }

    struct ViewData {
      typedef ViewData Self;
      struct Viewport {
        uint x;
        uint y;
        uint width;
        uint height;
      };
      PROP(Viewport, viewport, { });
      PROP(mat4, viewMatrix, mat4());
      PROP(mat4, projMatrix, mat4());
      PROP(gpu::Framebuffer, framebuffer, { });
      PROP_ARRAY(bool, cMaxFramebufferColorAttachments, colorAttachment);
      PROP(CubemapTarget, cubemapTarget, CubemapTarget::Invalid);
      PROP(uint, mipLevel, 0);
    };

    struct ClearData {
      typedef ClearData Self;
      PROP(Color, color, { 0.0f, 0.0f, 0.0f, 1.0f });
      PROP(float, depth, 1.0f);
      PROP(int32, stencil, 0);
      PROP(bool, clearColor, true);
      PROP(bool, clearDepth, true);
      PROP(bool, clearStencil, false);
    };

    struct FillBufferData {
      typedef FillBufferData Self;
      PROP(gpu::Buffer, buffer, { });
      PROP(uint, offset, 0);
      PROP(uint, size, 0);
      PROP_PTR(void, data);
    };

    struct FillTextureData {
      typedef FillTextureData Self;
      PROP(gpu::Texture, texture, { });
      PROP(uint16, offsetX, 0);
      PROP(uint16, offsetY, 0);
      PROP(uint16, offsetZ, 0);
      PROP(uint16, width, 0);
      PROP(uint16, height, 0);
      PROP(uint16, depth, 0);
      PROP(bool, buildMipmap, false);
      PROP_PTR(void, data0);
      PROP_PTR(void, data1);
      PROP_PTR(void, data2);
      PROP_PTR(void, data3);
      PROP_PTR(void, data4);
      PROP_PTR(void, data5);
    };

    struct SetupPipelineData {
      typedef SetupPipelineData Self;
      PROP(gpu::Pipeline, pipeline, { });
      PROP_ARRAY(gpu::Texture, cMaxTextureUnits, texture);
      PROP_ARRAY(gpu::Buffer, cMaxVertexAttribs, buffer);
      PROP_ARRAY(gpu::Buffer, cMaxUniformBuffers, uniformBuffer);
      PROP_ARRAY(Uniform, cMaxShaderUniforms, uniform);
      PROP(vec4, scissor, vec4(0.0f));
      PROP(mat4, modelMatrix, mat4());
    };

    struct RenderData {
      typedef RenderData Self;
      PROP(gpu::Buffer, indexBuffer, { });
      PROP(uint, offset, 0);
      PROP(uint, count, 0);
      PROP(uint, instances, 1);
      PROP(IndexFormat, type, IndexFormat::Uint16);
    };

#undef PROP
#undef PROP_PTR
#undef PROP_ARRAY

    ViewData &setupViewCommand();
    ClearData &clearCommand();
    FillBufferData &fillBufferCommand();
    FillTextureData &fillTextureCommand();
    SetupPipelineData &setupPipelineCommand();
    RenderData &renderCommand();

  private:
    vector<ref_ptr<Command>> commands_;
  };

}

#endif //XE_DISPLAY_LIST_HPP
