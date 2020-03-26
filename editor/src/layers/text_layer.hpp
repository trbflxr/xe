//
// Created by trbflxr on 3/23/2020.
//

#ifndef XE_TEXT_LAYER_HPP
#define XE_TEXT_LAYER_HPP

#include <xe/core/engine.hpp>
#include <xe/graphics/camera.hpp>
#include <xe/font.hpp>
#include <xe/graphics/renderer2d.hpp>

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
    Font font_;
    std::unique_ptr<OrthographicCamera> camera_;
    std::unique_ptr<Renderer2d> renderer_;

    std::shared_ptr<Texture> texture_;

    float *text_quad_vertexData_;

    struct {
      std::shared_ptr<gpu::Pipeline> pipeline;
      std::shared_ptr<gpu::Buffer> vertexBuff;
      std::shared_ptr<gpu::Buffer> indexBuff;
    } quad_;
  };

}

#endif //XE_TEXT_LAYER_HPP
