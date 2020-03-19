//
// Created by trbflxr on 3/18/2020.
//

#ifndef XE_RENDERER2D_LAYER_HPP
#define XE_RENDERER2D_LAYER_HPP

#include <xe/graphics/texture.hpp>
#include <xe/graphics/renderer2d.hpp>

#include "layer_base.hpp"

namespace xe {

  class Renderer2dLayer : public LayerBase {
  public:
    void onStart() override;

    void onUpdate() override;

    void onPreRender() override;
    void onRender() override;

    bool onUi() override;

  private:
    std::unique_ptr<OrthographicCamera> camera_;
    std::unique_ptr<Renderer2d> renderer_;
    uint32_t quadCount_ = 0;

    std::shared_ptr<Texture> texture_;
  };

}

#endif //XE_RENDERER2D_LAYER_HPP