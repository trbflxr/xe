//
// Created by trbflxr on 3/14/2020.
//

#include "xepch.hpp"
#include <xe/graphics/camera.hpp>

#include <xe/core/engine.hpp>

namespace xe {

  Camera::Camera() {
    setName("Camera");
  }

  void Camera::onUi() {
    Object::onUi();
  }

  void Camera::update() {
    if (dirty_) {
      XE_TRACE_BEGIN("XE", "Compute camera transformations");
      projection_ = mat4::perspective(fov_, aspect_, nearPlane_, farPlane_);
      view_ = mat4::transformation(transform_.worldPosition(), transform_.worldRotation(), transform_.worldScale());
      dirty_ = false;
      XE_TRACE_END("XE", "Compute camera transformations");
    }
  }

  mat4 Camera::makeProjection(float hFov, float aspect, float nearZ, float farZ) {
    return mat4::perspective(hFov, aspect, nearZ, farZ);
  }

}
