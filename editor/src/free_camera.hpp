//
// Created by trbflxr on 3/31/2020.
//

#ifndef XE_FREE_CAMERA_HPP
#define XE_FREE_CAMERA_HPP

#include <xe/core/event.hpp>
#include <xe/graphics/camera.hpp>

namespace xe {

  class FreeCamera : GPUResource {
  public:
    FreeCamera(const vec2u &resolution, float fovDeg, float aspect, float nearZ, float farZ,
               float moveSpeed = 4.0f, float sprintSpeed = 4.0f, float mouseSensitivity = 0.15f);

    void destroy() override;

    void onRender();
    void onUpdate();

    bool onKeyPressed(Event::Key key);
    bool onMousePressed(Event::MouseButton mb);
    bool onFocusLost();

    float getMoveSpeed() const { return moveSpeed_; }
    void setMoveSpeed(float speed) { moveSpeed_ = speed; }

    float getSprintSpeed() const { return sprintSpeed_; }
    void setSprintSpeed(float speed) { sprintSpeed_ = speed; }

    PerspectiveCamera &cam() { return camera_; }
    const PerspectiveCamera &cam() const { return camera_; }

    friend const Node &operator>>(const Node &node, FreeCamera &c) {
      node["moveSpeed"].get(c.moveSpeed_);
      node["sprintSpeed"].get(c.sprintSpeed_);
      node["mouseSensitivity"].get(c.mouseSensitivity_);
      return node;
    }

    friend Node &operator<<(Node &node, const FreeCamera &c) {
      node["moveSpeed"].set(c.moveSpeed_);
      node["sprintSpeed"].set(c.sprintSpeed_);
      node["mouseSensitivity"].set(c.mouseSensitivity_);
      return node;
    }

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
