//
// Created by trbflxr on 3/31/2020.
//

#include "free_camera.hpp"

#include <xe/core/engine.hpp>

namespace xe {

  FreeCamera::FreeCamera(const vec2u &resolution, float fovDeg, float aspect, float nearZ, float farZ,
                         float moveSpeed, float sprintSpeed, float mouseSensitivity) :
      camera_(resolution, fovDeg, aspect, nearZ, farZ),
      moveSpeed_(moveSpeed),
      sprintSpeed_(sprintSpeed),
      mouseSensitivity_(mouseSensitivity) {

    windowSize_ = Engine::ref().window().size();
    windowCenter_ = windowSize_ / 2.0f;
    lastMousePosition_ = windowCenter_;
  }

  void FreeCamera::destroy() {
    camera_.destroy();
  }

  void FreeCamera::onRender() {
    camera_.updateUniforms();
  }

  void FreeCamera::onUpdate() {
    auto ts = Engine::ref().delta();

    if (mouseLocked_) {
      vec2 mouseChange = Engine::getMousePosition() - lastMousePosition_;

      //rotate
      camera_.transform().rotate(quat(vec3::unitYN(), mouseChange.x * mouseSensitivity_), Transform::Space::Parent);
      camera_.transform().rotate(quat(-camera_.transform().localRight(), mouseChange.y * mouseSensitivity_), Transform::Space::Parent);


      Engine::executeInRenderThread([this]() {
        lastMousePosition_ = Engine::getMousePosition();
        Engine::setMousePosition(windowCenter_);
      });

      //move
      float speed = Engine::isKeyPressed(Keyboard::LControl) ? moveSpeed_ * sprintSpeed_ : moveSpeed_;
      if (Engine::isKeyPressed(Keyboard::W)) {
        camera_.transform().translate(camera_.transform().localForward() * speed * ts, Transform::Space::Parent);
      }
      if (Engine::isKeyPressed(Keyboard::S)) {
        camera_.transform().translate(-camera_.transform().localForward() * speed * ts, Transform::Space::Parent);
      }

      if (Engine::isKeyPressed(Keyboard::A)) {
        camera_.transform().translate(-camera_.transform().localRight() * speed * ts, Transform::Space::Parent);
      }
      if (Engine::isKeyPressed(Keyboard::D)) {
        camera_.transform().translate(camera_.transform().localRight() * speed * ts, Transform::Space::Parent);
      }

      if (Engine::isKeyPressed(Keyboard::Space)) {
        camera_.transform().translate(vec3::unitY() * speed * ts, Transform::Space::Parent);
      }
      if (Engine::isKeyPressed(Keyboard::LShift)) {
        camera_.transform().translate(vec3::unitYN() * speed * ts, Transform::Space::Parent);
      }
    }
  }

  bool FreeCamera::onKeyPressed(Event::Key key) {
    if (key.code == Keyboard::Escape || key.code == Keyboard::R) {
      mouseLocked_ = false;
      return true;
    }
    return false;
  }

  bool FreeCamera::onMousePressed(Event::MouseButton mb) {
    if (mb.button == Mouse::Right) {
      mouseLocked_ = !mouseLocked_;
      if (mouseLocked_) {
        Engine::executeInRenderThread([this]() {
          Engine::setMousePosition(windowCenter_);
        });
      }
      Engine::ref().window().setCursorVisible(!mouseLocked_);
      return true;
    }
    return false;
  }

  bool FreeCamera::onFocusLost() {
    mouseLocked_ = false;
    Engine::ref().window().setCursorVisible(!mouseLocked_);
    return false;
  }

}
