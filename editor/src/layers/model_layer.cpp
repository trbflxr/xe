//
// Created by trbflxr on 3/29/2020.
//

#include <xe/ui/ui.hpp>
#include "model_layer.hpp"

#include <xe/utils/logger.hpp>
#include <xe/serialization/file.hpp>

namespace xe {

  void ModelLayer::onStart() {
//    model_ = std::make_shared<Model>("assets/models/Suzanne/Suzanne.gltf");
    model_ = std::make_shared<Model>("assets/models/Sponza/Sponza.gltf");
//    model_ = std::make_shared<Model>("assets/models/monkey3.obj");

    camera_ = std::make_unique<FreeCamera>(vec2(1280, 720), 70.0f, 1280.0f / 720.0f, 1.0f, 1000.0f);
    camera_->cam().transform().setLocalPosition({0.0f, 0.0f, 2.0f});

    File camFile("camera.json");
    if (camFile.load()) {
      *camFile.getNode() >> *camera_;
    }
  }

  void ModelLayer::onStop() {
    File camFile("camera.json");
    *camFile.getNode() << *camera_;
    camFile.write(xe::Node::Format::Beautified);

    camera_->destroy();
    model_->destroy();
  }

  void ModelLayer::onRender() {
    camera_->onRender();

    DisplayList frame;
    frame.setupViewCommand()
        .set_viewport(camera_->cam().viewport())
        .set_framebuffer(Engine::ref().composer().framebuffer())
        .set_colorAttachment(0, true);
    frame.clearCommand()
        .set_clearColor(false)
        .set_clearDepth(true);

    model_->render(frame, camera_->cam());

    Engine::ref().executeOnGpu(std::move(frame));
  }

  void ModelLayer::onUpdate() {
    camera_->onUpdate();
  }

  bool ModelLayer::onKeyPressed(Event::Key key) {
    return camera_->onKeyPressed(key);
  }

  bool ModelLayer::onMousePressed(Event::MouseButton button) {
    return camera_->onMousePressed(button);
  }

  bool ModelLayer::onFocusLost() {
    return camera_->onFocusLost();
  }

  bool ModelLayer::onUi() {
    static float ms = camera_->getMoveSpeed();
    static float ss = camera_->getSprintSpeed();

    ImGui::Text("ModelLayer:");
    if (ImGui::SliderFloat("MoveSpeed", &ms, 1.0f, 100.0f)) {
      camera_->setMoveSpeed(ms);
    }
    if (ImGui::SliderFloat("SprintSpeed", &ss, 1.0f, 100.0f)) {
      camera_->setSprintSpeed(ss);
    }
    return false;
  }

}
