//
// Created by trbflxr on 3/23/2020.
//

#ifndef XE_TEXT_LAYER_HPP
#define XE_TEXT_LAYER_HPP

#include <xe/core/engine.hpp>
#include <xe/graphics/camera.hpp>
#include <xe/font.hpp>
#include <xe/graphics/renderer2d.hpp>
#include <xe/graphics/text_renderer.hpp>

#include "layer_base.hpp"

namespace xe {

  class TextLayer : public LayerBase {
  public:
    void onStart() override;
    void onStop() override;

    void onPreRender() override;
    void onRender() override;
    void onUpdate() override;

    bool onKeyPressed(Event::Key e) override;
    bool onTextEntered(Event::Text text) override;

    bool onUi() override;

  private:
    bool renderAtlas_ = false;

    float textSize_ = 0.0f;
    float textScale_ = 0.0f;
    float textOutline_ = 0.0f;
    Color color_;
    Color outlineColor_;

    std::shared_ptr<Text> text_;
    std::shared_ptr<Font> font_;
    std::unique_ptr<OrthographicCamera> camera_;
    std::unique_ptr<Renderer2d> renderer_;
    std::unique_ptr<TextRenderer> textRenderer_;
  };

}

#endif //XE_TEXT_LAYER_HPP
