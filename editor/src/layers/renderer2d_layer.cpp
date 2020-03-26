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
    quadCount_ = 50;

    gpu::Texture::Info info;
    info.minFilter = TextureMinFilter::Linear;
    info.magFilter = TextureMagFilter::Linear;
    info.bindless = true;

    texture_ = std::make_shared<Texture>();
    texture_->setInfo(info);
    texture_->loadFromFile("textures/test.png");
    texture_->setup();

    texture1_ = std::make_shared<Texture>();
    texture1_->setInfo(info);
    texture1_->loadFromFile("textures/test1.png");
    texture1_->setup();
  }

  void Renderer2dLayer::onStop() {
    camera_->destroy();
    texture_->destroy();
    texture1_->destroy();
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
  }

  void Renderer2dLayer::onPreRender() {
    camera_->updateUniforms();
  }

  void Renderer2dLayer::onRender() {
    renderer_->begin();

    static const float size = 0.1f;
    static const float offset = size + 0.02f;

    float x = 0.0f;
    float y = 0.0f;
    uint32_t texSwitch = 0;
    for (uint32_t i = 0; i < quadCount_; ++i) {
      if (i % 200 == 0) {
        x = 0.0f;
        y += offset;
      }

      if (texSwitch < switchTextures_) {
        renderer_->submit({x, y}, {size, size}, Color::Olive, (texSwitch % 2 == 0) ? texture_ : texture1_);
        ++texSwitch;
      } else {
        renderer_->submit({x, y}, {size, size}, Color::Red, (i % 2 == 0) ? texture1_ : nullptr);
      }

      x += offset;
    }

    renderer_->end();
  }

  bool Renderer2dLayer::onUi() {
    std::string vendor;
    if (Engine::ref().gpu().vendor() == GPU::Vendor::AMD) {
      vendor = "AMD";
    } else if (Engine::ref().gpu().vendor() == GPU::Vendor::Nvidia) {
      vendor = "Nvidia";
    } else if (Engine::ref().gpu().vendor() == GPU::Vendor::Intel) {
      vendor = "Intel";
    } else {
      vendor = "Invalid";
    }

    ImGui::Text("Renderer2dLayer:");
    ImGui::Text("GPU vendor: %s", vendor.c_str());
    ImGui::SliderInt("Switch textures", reinterpret_cast<int32_t *>(&switchTextures_), 1, 10000);
    ImGui::SliderInt("Quad count", reinterpret_cast<int32_t *>(&quadCount_), 1, renderer_->maxInstances());
    return false;
  }

}
