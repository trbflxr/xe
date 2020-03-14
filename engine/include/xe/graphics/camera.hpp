//
// Created by trbflxr on 3/14/2020.
//

#ifndef XE_CAMERA_HPP
#define XE_CAMERA_HPP

#include <xe/core/object.hpp>
#include <xe/graphics/color.hpp>
#include <xe/graphics/gpu_resources.hpp>
#include <xe/math/transform.hpp>

namespace xe {

  class Camera : public Object {
  XE_OBJECT(Camera, Object);
  public:
    struct ClearFlags {
      enum Enum {
        SolidColor,
        Skybox,
      };
    };

    struct CommonData {
      mat4 proj;
      mat4 view;
    };

  public:
    Camera();

    void onUi() override;

#define PROPERTY(type, name, fname, ...)                            \
     private:                                                       \
      type name = __VA_ARGS__;                                      \
     public:                                                        \
      void set_##fname(const type &c) { name = c; markForUpdate(); }\
      type fname() const { return name; }

  PROPERTY(float, fov_, fov, 70.0f);
  PROPERTY(float, aspect_, aspect, 1280.0f / 720.0f);
  PROPERTY(float, nearPlane_, nearPlane, 1.0f);
  PROPERTY(float, farPlane_, farPlane, 1000.0f);
  PROPERTY(Color, backgroundColor_, backgroundColor, Color::Teal);
  PROPERTY(bool, clearColor_, clearColor, true);
  PROPERTY(bool, clearDepth_, clearDepth, true);
  PROPERTY(bool, clearStencil_, clearStencil, true);
#undef PROPERTY

    void setClearFlags(ClearFlags::Enum flags) { clearFlags_ = flags; }
    ClearFlags::Enum clearFlags() const { return clearFlags_; }

    const mat4 &projection() const { return projection_; }
    const mat4 &view() const { return view_; }
    Transform &transform() { return transform_; }

    void markForUpdate() { dirty_ = true; }
    bool hasChanged() const { return dirty_; }

    void update();

    static mat4 makeProjection(float hFov, float aspect, float nearZ, float farZ);

  private:
    bool dirty_ = true;
    ClearFlags::Enum clearFlags_ = ClearFlags::SolidColor;

    mat4 projection_;
    mat4 view_;

    Transform transform_;
  };

}

#endif //XE_CAMERA_HPP
