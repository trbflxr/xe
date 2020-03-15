//
// Created by trbflxr on 3/14/2020.
//

#ifndef XE_CAMERA_HPP
#define XE_CAMERA_HPP

#include <xe/core/object.hpp>
#include <xe/graphics/color.hpp>
#include <xe/graphics/composer.hpp>
#include <xe/math/transform.hpp>

namespace xe {

  class XE_API Camera : public Object {
  XE_OBJECT(Camera, Object);
  public:
    struct ClearFlags {
      enum Enum {
        SolidColor,
        Skybox,
      };
    };

    struct CommonData {
      mat4 model;
      mat4 view;
      mat4 proj;
      vec4 resolution;
    };

  public:
    explicit Camera(const vec2u &resolution);

    void init();

    void onPreRender();

    void onUi() override;

#define PROPERTY(type, name, fname, update, ...)                                    \
     private:                                                                       \
      type name = __VA_ARGS__;                                                      \
     public:                                                                        \
      void set_##fname(const type &c) { name = c; if(update) { markForUpdate(); } } \
      type fname() const { return name; }

  PROPERTY(float, fov_, fov, true, 70.0f);
  PROPERTY(float, aspect_, aspect, true, 1280.0f / 720.0f);
  PROPERTY(float, nearPlane_, nearPlane, true, 1.0f);
  PROPERTY(float, farPlane_, farPlane, true, 1000.0f);
  PROPERTY(Color, backgroundColor_, backgroundColor, false, Color::Teal);
  PROPERTY(bool, clearColor_, clearColor, false, true);
  PROPERTY(bool, clearDepth_, clearDepth, false, true);
  PROPERTY(bool, clearStencil_, clearStencil, false, true);
#undef PROPERTY

    void setClearFlags(ClearFlags::Enum flags) { clearFlags_ = flags; }
    ClearFlags::Enum clearFlags() const { return clearFlags_; }

    const vec2u &viewport() const { return resolution_; }

    const mat4 &projection() const { return projection_; }
    const mat4 &view() const { return view_; }
    Transform &transform() { return transform_; }
    Composer &composer() { return composer_; }

    const gpu::Buffer &uniformBuffer() const { return uniforms_; }

    void markForUpdate() { dirty_ = true; }
    bool hasChanged() const { return dirty_; }

    void update();

    static mat4 makeProjection(float hFov, float aspect, float nearZ, float farZ);

  private:
    bool dirty_ = true;
    ClearFlags::Enum clearFlags_ = ClearFlags::SolidColor;

    mat4 projection_;
    mat4 view_;

    vec2u resolution_;
    Transform transform_;

    Composer composer_;

    CommonData data_;
    gpu::Buffer uniforms_;
  };

}

#endif //XE_CAMERA_HPP
