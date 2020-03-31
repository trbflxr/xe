//
// Created by trbflxr on 3/29/2020.
//

#ifndef XE_MODEL_LAYER_HPP
#define XE_MODEL_LAYER_HPP

#include <xe/core/engine.hpp>
#include <xe/graphics/camera.hpp>
#include <xe/graphics/texture.hpp>
#include <xe/graphics/model.hpp>
#include "layer_base.hpp"

namespace xe {

  class ModelLayer : public LayerBase {
  public:
    void onStart() override;
    void onStop() override;

    void onPreRender() override;
    void onRender() override;
    
    void onUpdate() override;

    bool onKeyPressed(Event::Key key) override;

    bool onUi() override;

  private:
    std::unique_ptr<PerspectiveCamera> camera_;
    vec3 cameraPos_;

    std::shared_ptr<Model> model_;
    struct {
      std::shared_ptr<gpu::Pipeline> pipeline;
      std::shared_ptr<gpu::Buffer> vertexBuffer;
      std::shared_ptr<gpu::Buffer> indexBuffer;
      std::shared_ptr<Texture> texture;
    } modelData_;
  };

}

#endif //XE_MODEL_LAYER_HPP
