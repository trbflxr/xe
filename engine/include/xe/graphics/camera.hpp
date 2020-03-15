//
// Created by trbflxr on 3/14/2020.
//

#ifndef XE_CAMERA_HPP
#define XE_CAMERA_HPP

#include <xe/core/object.hpp>
#include <xe/graphics/color.hpp>
#include <xe/graphics/composer.hpp>
#include <xe/graphics/display_list.hpp>
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
    void updateUniforms();

    Color getBackgroundColor() const { return backgroundColor_; }
    void setBackgroundColor(const Color &color);

    bool clearColor() const { return clearColor_; }
    void setClearColor(bool clear);

    bool clearDepth() const { return clearDepth_; }
    void setClearDepth(bool clear);

    bool clearStencil() const { return clearStencil_; }
    void setClearStencil(bool clear);

    void setClearFlags(ClearFlags::Enum flags) { clearFlags_ = flags; }
    ClearFlags::Enum clearFlags() const { return clearFlags_; }

    const vec2u &resolution() const { return resolution_; }
    uint32_t width() const { return resolution_.x; }
    uint32_t height() const { return resolution_.y; }

    DisplayList::ViewData::Viewport viewport() const { return {0, 0, resolution_.x, resolution_.y}; }

    const mat4 &projection() const { return projection_; }
    const mat4 &view() const { return view_; }
    Transform &transform() { return transform_; }

    const gpu::Buffer &uniformBuffer() const { return uniforms_; }

    void markForUpdate() { dirty_ = true; }
    bool hasChanged() const { return dirty_; }

    virtual void update() = 0;

  protected:
    void init();

  protected:
    Color backgroundColor_ = Color::Clear;
    bool clearColor_ = true;
    bool clearDepth_ = true;
    bool clearStencil_ = true;

    bool dirty_ = true;
    ClearFlags::Enum clearFlags_ = ClearFlags::SolidColor;

    mat4 projection_;
    mat4 view_;

    vec2u resolution_;
    Transform transform_;

    CommonData data_;
    gpu::Buffer uniforms_;
  };

  class XE_API OrthographicCamera : public Camera {
  XE_OBJECT(OrthographicCamera, Camera);
  public:
    explicit OrthographicCamera(const vec2u &resolution, float left, float right, float bottom, float top, float nearPlane, float farPlane);

    float left() const { return left_; }
    void setLeft(float left);

    float right() const { return right_; }
    void setRight(float right);

    float bottom() const { return bottom_; }
    void setBottom(float bottom);

    float top() const { return top_; }
    void setTop(float top);

    float nearPlane() const { return nearPlane_; }
    void setNearPlane(float nearPlane);

    float farPlane() const { return farPlane_; }
    void setFarPlane(float farPlane);

    void update() override;

  private:
    float left_;
    float right_;
    float bottom_;
    float top_;
    float nearPlane_;
    float farPlane_;
  };

  class XE_API PerspectiveCamera : public Camera {
  XE_OBJECT(PerspectiveCamera, Camera);
  public:
    explicit PerspectiveCamera(const vec2u &resolution, float fovDeg, float aspect, float nearZ, float farZ);

    float fov() const { return fov_; }
    void setFov(float fov);

    float aspect() const { return aspect_; }
    void setAspect(float aspect);

    float nearPlane() const { return nearPlane_; }
    void setNearPlane(float nearPlane);

    float farPlane() const { return farPlane_; }
    void setFarPlane(float farPlane);

    void update() override;

  private:
    float fov_;
    float aspect_;
    float nearPlane_;
    float farPlane_;
  };

}

#endif //XE_CAMERA_HPP
