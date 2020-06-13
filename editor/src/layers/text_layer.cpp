//
// Created by trbflxr on 3/23/2020.
//

#include "text_layer.hpp"

#include <xe/utils/logger.hpp>
#include <xe/ui/ui.hpp>

namespace xe {

  void TextLayer::onStart() {
    camera_ = std::make_unique<OrthographicCamera>(vec2(1280, 720), 0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
    camera_->setBackgroundColor(Color::Purple);

    renderer_ = std::make_unique<Renderer2d>(*camera_);
    textRenderer_ = std::make_unique<TextRenderer>(1280, 720);

    font_ = std::make_shared<Font>();
    font_->loadFromFile("assets/fonts/consolata.otf", Charset::Cyrillic);

    text_ = std::make_shared<Text>("жопа test", font_, vec2(400.0f, 250.0f), 32.0f);
    text_->setColor(Color::Black);
    text_->setOutlineColor(Color::Red);
    text_->setSize(128.0f);

    textSize_ = text_->size();
    textScale_ = text_->scale();
    textOutline_ = text_->outlineWidth();
    color_ = text_->color();
    outlineColor_ = text_->outlineColor();
  }

  void TextLayer::onStop() {
    font_->destroy();
    camera_->destroy();
    renderer_->destroy();
  }

  void TextLayer::onPreRender() {
    camera_->updateUniforms();
  }

  void TextLayer::onRender() {
    if (renderAtlas_) {
      renderer_->begin();
      renderer_->submit({0.0f, 0.0f}, font_->atlas()->size(), Color::White, font_->atlas());
      renderer_->end();
    }

    textRenderer_->begin();
    textRenderer_->submit(*text_);
    textRenderer_->end();
  }

  void TextLayer::onUpdate() {
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

  bool TextLayer::onKeyPressed(Event::Key e) {
    return false;
  }

  bool TextLayer::onTextEntered(Event::Text text) {
//    XE_CORE_CRITICAL("[TextLayer] unicode: {}", text.unicode);
    return false;
  }

  bool TextLayer::onUi() {
    ImGui::Text("TextLayer:");
    ImGui::Checkbox("Render atlas", &renderAtlas_);

    if (ImGui::SliderFloat("Text size:", &textSize_, 1.0f, 256.0f)) {
      text_->setSize(textSize_);
    }
    if (ImGui::SliderFloat("Text scale:", &textScale_, 1.0f, 10.0f)) {
      text_->setScale(textScale_);
    }
    if (ImGui::SliderFloat("Text outline width:", &textOutline_, 0.0f, 1.0f)) {
      text_->setOutlineWidth(textOutline_);
    }
    if (ImGui::SliderFloat("Text outline edge:", &textOutlineEdge_, 0.0f, 1.0f)) {
      text_->setOutlineEdge(textOutlineEdge_);
    }
    if (ImGui::ColorEdit3("Color: ", reinterpret_cast<float *>(&color_))) {
      text_->setColor(color_);
    }
    if (ImGui::ColorEdit3("Outline color: ", reinterpret_cast<float *>(&outlineColor_))) {
      text_->setOutlineColor(outlineColor_);
    }

    char buff[256];
    std::memset(buff, 0, 256);
    std::memcpy(buff, text_->string().data(), text_->string().size());
    if (ImGui::InputText("String: ", buff, 256)) {
      text_->setString(buff);
    }
    return false;
  }

}

