//
// Created by trbflxr on 3/29/2020.
//

#ifndef XE_MODEL_LAYER_HPP
#define XE_MODEL_LAYER_HPP

#include <xe/core/engine.hpp>
#include <xe/graphics/texture.hpp>
#include <xe/graphics/model.hpp>
#include "layer_base.hpp"
#include "free_camera.hpp"

namespace xe {

  class ModelLayer : public LayerBase {
  public:
    void onStart() override;
    void onStop() override;

    void onRender() override;
    
    void onUpdate() override;

    bool onKeyPressed(Event::Key key) override;
    bool onMousePressed(Event::MouseButton button) override;
    bool onFocusLost() override;

    bool onUi() override;

  private:
    std::unique_ptr<FreeCamera> camera_;

    std::shared_ptr<Model> model_;
  };

}

#endif //XE_MODEL_LAYER_HPP
