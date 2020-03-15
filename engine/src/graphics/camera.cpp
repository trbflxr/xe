//
// Created by trbflxr on 3/14/2020.
//

#include "xepch.hpp"
#include <xe/graphics/camera.hpp>

#include <xe/core/engine.hpp>

namespace xe {

  Camera::Camera(const vec2u &resolution) :
      resolution_(resolution),
      composer_(resolution_) {
    setName("Camera");

  }

  void Camera::init() {
    uniforms_ = Engine::ref().gpu().createBuffer({BufferType::Uniform, Usage::Dynamic, sizeof(data_), "CameraUniform", cCameraUboBinding});

    DisplayList frame;
    frame.fillBufferCommand()
        .set_buffer(uniforms_)
        .set_data(&data_)
        .set_size(sizeof(data_));
    Engine::ref().submitDrawList(std::move(frame));

    composer_.init();
  }

  void Camera::onPreRender() {
    DisplayList frame;
    frame.fillBufferCommand()
        .set_buffer(uniforms_)
        .set_data(&data_)
        .set_size(sizeof(data_));
    Engine::ref().submitDrawList(std::move(frame));
  }

  void Camera::onUi() {
    Object::onUi();
  }

  void Camera::update() {
    if (dirty_) {
      XE_TRACE_BEGIN("XE", "Compute camera transformations");
      projection_ = mat4::perspective(fov_, aspect_, nearPlane_, farPlane_);
      view_ = mat4::transformation(transform_.worldPosition(), transform_.worldRotation(), transform_.worldScale());

      data_.view = view_;
      data_.proj = projection_;
      data_.model = transform_.worldTransform();
//      data_.resolution = vec4(resolution_);

      dirty_ = false;
      XE_TRACE_END("XE", "Compute camera transformations");
    }
  }

  mat4 Camera::makeProjection(float hFov, float aspect, float nearZ, float farZ) {
    return mat4::perspective(hFov, aspect, nearZ, farZ);
  }

}
