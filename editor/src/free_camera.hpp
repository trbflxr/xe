//
// Created by trbflxr on 3/31/2020.
//

#ifndef XE_FREE_CAMERA_HPP
#define XE_FREE_CAMERA_HPP

#include <xe/core/event.hpp>
#include <xe/graphics/camera.hpp>

namespace xe {

  class FreeCamera {
  public:
    FreeCamera(const vec2u &resolution, float fovDeg, float aspect, float nearZ, float farZ,
               float moveSpeed = 4.0f, float sprintSpeed = 4.0f, float mouseSensitivity = 0.15f);

    void destroy();

    void onRender();
    void onUpdate();

    bool onKeyPressed(Event::Key key);
    bool onMousePressed(Event::MouseButton mb);
    bool onFocusLost();

    PerspectiveCamera &cam() { return camera_; }
    const PerspectiveCamera &cam() const { return camera_; }

  private:
    float moveSpeed_ = 0.0f;
    float sprintSpeed_ = 0.0f;
    float mouseSensitivity_ = 0.0f;

    bool mouseLocked_ = false;
    vec2 windowSize_;
    vec2 windowCenter_;
    vec2 lastMousePosition_;

    PerspectiveCamera camera_;
  };

}

#endif //XE_FREE_CAMERA_HPP
