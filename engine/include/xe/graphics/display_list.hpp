//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_DISPLAY_LIST_HPP
#define XE_DISPLAY_LIST_HPP

#include <memory>
#include <vector>
#include <xe/core/object.hpp>
#include <xe/graphics/color.hpp>
#include <xe/graphics/gpu_resources.hpp>

namespace xe {

  struct Uniform {
    const char *name = nullptr;
    uint8_t data[64] = { };

    Uniform() : name(nullptr) { }

    Uniform(const char *name, const void *data, size_t size) : name(name) {
      std::memcpy(Uniform::data, data, size);
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
    Self &set_v_##name(const std::vector<type> &c) { for (uint32_t i = 0; i < c.size(); ++i) { set_##name(i, c[i]); } return *this; }

    struct ViewData {
      typedef ViewData Self;
      struct Viewport {
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
      };
      PROP(Viewport, viewport, { });
      PROP(gpu::Framebuffer, framebuffer, { });
      PROP_ARRAY(bool, cMaxFramebufferColorAttachments, colorAttachment);
      PROP(CubemapTarget, cubemapTarget, CubemapTarget::Invalid);
      PROP(uint32_t, mipLevel, 0);
    };

    struct ClearData {
      typedef ClearData Self;
      PROP(Color, color, { 0.0f, 0.0f, 0.0f, 1.0f });
      PROP(float, depth, 1.0f);
      PROP(int32_t, stencil, 0);
      PROP(bool, clearColor, true);
      PROP(bool, clearDepth, true);
      PROP(bool, clearStencil, false);
    };

    struct FillBufferData {
      typedef FillBufferData Self;
      PROP(gpu::Buffer, buffer, { });
      PROP(uint32_t, offset, 0);
      PROP(uint32_t, size, 0);
      PROP_PTR(void, data);
    };

    struct FillTextureData {
      typedef FillTextureData Self;
      PROP(gpu::Texture, texture, { });
      PROP(uint32_t, offsetX, 0);
      PROP(uint32_t, offsetY, 0);
      PROP(uint32_t, offsetZ, 0);
      PROP(uint32_t, width, 0);
      PROP(uint32_t, height, 0);
      PROP(uint32_t, depth, 0);
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
    };

    struct RenderData {
      typedef RenderData Self;
      PROP(gpu::Buffer, indexBuffer, { });
      PROP(uint32_t, offset, 0);
      PROP(uint32_t, count, 0);
      PROP(uint32_t, instances, 1);
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
    std::vector<std::shared_ptr<Command>> commands_;
  };

}

#endif //XE_DISPLAY_LIST_HPP
