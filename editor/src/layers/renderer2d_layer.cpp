//
// Created by trbflxr on 3/18/2020.
//

#include "renderer2d_layer.hpp"

#include <xe/core/engine.hpp>
#include <xe/ui/imgui/imgui.h>
#include <xe/utils/logger.hpp>

namespace xe {

  void Renderer2dLayer::onStart() {
    camera_ = std::make_unique<OrthographicCamera>(vec2(1280, 720), -12.8f, 12.8f, -7.2f, 7.2f, -1.0f, 1.0f);
    camera_->setBackgroundColor(Color::Purple);

    renderer_ = std::make_unique<Renderer2d>(*camera_);
    quadCount_ = renderer_->maxInstances();

    gpu::Texture::Info info;
    info.minFilter = TextureMinFilter::Linear;
    info.magFilter = TextureMagFilter::Linear;

    texture_ = std::make_shared<Texture>();
    texture_->setInfo(info);
    texture_->loadFromFile("textures/test.png");
    texture_->setup();
  }

  void Renderer2dLayer::onStop() {
    camera_->destroy();
    texture_->destroy();
    renderer_->destroy();
  }

  void Renderer2dLayer::onUpdate() {
    static float speed = 10.0f;

    auto ts = Engine::ref().delta();

    if (Engine::isKeyPressed(Keyboard::S)) {
      camera_->transform().translateY(speed * ts);
    } else if (Engine::isKeyPressed(Keyboard::W)) {
      camera_->transform().translateY(-speed * ts);
    }

    if (Engine::isKeyPressed(Keyboard::D)) {
      camera_->transform().translateX(-speed * ts);
    } else if (Engine::isKeyPressed(Keyboard::A)) {
      camera_->transform().translateX(speed * ts);
    }

    camera_->update();
  }

  void Renderer2dLayer::onPreRender() {
    camera_->updateUniforms();
  }

  void Renderer2dLayer::onRender() {
    renderer_->begin();

    static const float size = 0.2f;
    static const float offset = size + 0.05f;

    float x = 0.0f;
    float y = 0.0f;
    for (uint32_t i = 0; i < quadCount_; ++i) {
      if (i % 100 == 0) {
        x = 0.0f;
        y += offset;
      }

      renderer_->submit({x, y}, {size, size}, Color::Olive, texture_);

      x += offset;
    }

    renderer_->end();
    renderer_->flush();
  }

  bool Renderer2dLayer::onUi() {
    ImGui::Text("Renderer2dLayer:");
    if (ImGui::SliderInt("Max quads", reinterpret_cast<int32_t *>(&quadCount_), 1, renderer_->maxInstances())) {
//      renderer_->setMaxInstances(quadCount_);
    }
    return false;
  }

}
