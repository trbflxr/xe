//
// Created by trbflxr on 3/18/2020.
//

#include <xe/core/engine.hpp>
#include <xe/ui/imgui/imgui.h>

#include "renderer2d_layer.hpp"

namespace xe {

  void Renderer2dLayer::onStart() {
    camera_ = std::make_unique<OrthographicCamera>(vec2(1280, 720), -10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
    camera_->setBackgroundColor(Color::Purple);

    renderer_ = std::make_unique<Renderer2d>(*camera_);


    gpu::Texture::Info info;
    info.minFilter = TextureMinFilter::Linear;
    info.magFilter = TextureMagFilter::Linear;

    texture_ = std::make_shared<Texture>();
    texture_->setInfo(info);
    texture_->loadFromFile("textures/test.png");
    texture_->setup();
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

    for (int32_t x = 0; x < 1280; x += 8) {
      for (int32_t y = 0; y < 720; y += 8) {
        renderer_->submit({x + 3.0f, y + 3.0f}, {6.0f, 6.0f}, Color::Olive, texture_);
      }
    }

    renderer_->end();
    renderer_->flush();
  }

  bool Renderer2dLayer::onUi() {
    ImGui::Text("Renderer2dLayer:");
    return false;
  }

}
