//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_TEST_OVERLAY_HPP
#define XE_TEST_OVERLAY_HPP

#include <xe/core/engine.hpp>
#include <xe/graphics/camera.hpp>
#include "layer_base.hpp"

namespace xe {

  struct Quad {
    gpu::Pipeline material;
    gpu::Buffer vertexBuff;
    gpu::Buffer indexBuff;
    gpu::Texture texture;
  };

  struct Renderer {

  };

  class TestOverlay : public LayerBase {
  public:
    explicit TestOverlay();

    void onStart() override;
    void onStop() override;

    void onRender() override;
    void onUpdate() override;

    bool onKeyPressed(Event::Key e) override;

    bool onUi() override;

  private:
    void *texData_ = nullptr;

    std::unique_ptr<OrthographicCamera> camera_;
    vec2 cameraPos_;
  };

}

#endif //XE_TEST_OVERLAY_HPP
