//
// Created by trbflxr on 3/14/2020.
//

#include "xepch.hpp"
#include <xe/graphics/camera.hpp>

#include <xe/core/engine.hpp>

namespace xe {

  void Camera::init() {
    uniforms_ = Engine::ref().gpu().createBuffer({BufferType::Uniform, Usage::Dynamic, sizeof(data_), "CameraUniform", cCameraUboBinding});

    DisplayList frame;
    frame.fillBufferCommand()
        .set_buffer(uniforms_)
        .set_data(&data_)
        .set_size(sizeof(data_));
    Engine::ref().submitDrawList(std::move(frame));
  }

  void Camera::updateUniforms() {
    DisplayList frame;
    frame.fillBufferCommand()
        .set_buffer(uniforms_)
        .set_data(&data_)
        .set_size(sizeof(data_));
    Engine::ref().submitDrawList(std::move(frame));
  }


  void Camera::setBackgroundColor(const Color &color) {
    backgroundColor_ = color;
    markForUpdate();
  }

  void Camera::setClearColor(bool clear) {
    clearColor_ = clear;
    markForUpdate();
  }

  void Camera::setClearDepth(bool clear) {
    clearDepth_ = clear;
    markForUpdate();
  }

  void Camera::setClearStencil(bool clear) {
    clearStencil_ = clear;
    markForUpdate();
  }

  //ortho
  OrthographicCamera::OrthographicCamera(const vec2u &resolution, float left, float right, float bottom, float top, float nearPlane, float farPlane) :
      left_(left),
      right_(right),
      bottom_(bottom),
      top_(top),
      nearPlane_(nearPlane),
      farPlane_(farPlane) {
    setName("OrthographicCamera");

    dirty_ = true;
    resolution_ = resolution;
    init();
  }

  void OrthographicCamera::setLeft(float left) {
    left_ = left;
    markForUpdate();
  }

  void OrthographicCamera::setRight(float right) {
    right_ = right;
    markForUpdate();
  }

  void OrthographicCamera::setBottom(float bottom)  {
    bottom_ = bottom;
    markForUpdate();
  }

  void OrthographicCamera::setTop(float top) {
    top_ = top;
    markForUpdate();
  }

  void OrthographicCamera::setNearPlane(float nearPlane)  {
    nearPlane_ = nearPlane;
    markForUpdate();
  }

  void OrthographicCamera::setFarPlane(float farPlane){
    farPlane_ = farPlane;
    markForUpdate();
  }

  void OrthographicCamera::update() {
    if (dirty_ || transform_.hasChanged()) {
      XE_TRACE_BEGIN("XE", "Compute OrthographicCamera transformations");
      projection_ = mat4::ortho(left_, right_, bottom_, top_, nearPlane_, farPlane_);
      view_ = mat4::transformation(transform_.worldPosition(), transform_.worldRotation(), transform_.worldScale());

      data_.view = view_;
      data_.proj = projection_;
      data_.model = transform_.worldTransform();
      data_.resolution = vec4(resolution_);

      dirty_ = false;
      XE_TRACE_END("XE", "Compute OrthographicCamera transformations");
    }
  }

  //perspective
  PerspectiveCamera::PerspectiveCamera(const vec2u &resolution, float fovDeg, float aspect, float nearZ, float farZ) :
      fov_(fovDeg),
      aspect_(aspect),
      nearPlane_(nearZ),
      farPlane_(farZ) {
    setName("PerspectiveCamera");

    dirty_ = true;
    resolution_ = resolution;
    init();
  }

  void PerspectiveCamera::setFov(float fov) {
    fov_ = fov;
    markForUpdate();
  }

  void PerspectiveCamera::setAspect(float aspect) {
    aspect_ = aspect;
    markForUpdate();
  }

  void PerspectiveCamera::setNearPlane(float nearPlane)  {
    nearPlane_ = nearPlane;
    markForUpdate();
  }

  void PerspectiveCamera::setFarPlane(float farPlane) {
    farPlane_ = farPlane;
    markForUpdate();
  }

  void PerspectiveCamera::update() {
    if (dirty_ || transform_.hasChanged()) {
      XE_TRACE_BEGIN("XE", "Compute PerspectiveCamera transformations");
      projection_ = mat4::perspective(fov_, aspect_, nearPlane_, farPlane_);
      view_ = mat4::transformation(transform_.worldPosition(), transform_.worldRotation(), transform_.worldScale());

      data_.view = view_;
      data_.proj = projection_;
      data_.model = transform_.worldTransform();
      data_.resolution = vec4(resolution_);

      dirty_ = false;
      XE_TRACE_END("XE", "Compute PerspectiveCamera transformations");
    }
  }

}
