//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_TEST_LAYER_HPP
#define XE_TEST_LAYER_HPP

#include <xe/core/engine.hpp>
#include <xe/graphics/camera.hpp>
#include "layer_base.hpp"

namespace xe {

  class TestLayer : public LayerBase {
  public:
    explicit TestLayer();

    void onStart() override;
    void onStop() override;

    void onRender() override;
    void onUpdate() override;

    bool onKeyPressed(Event::Key e) override;

    bool onUi() override;

  private:
    static constexpr int32_t INSTANCES = 50000;

    std::unique_ptr<PerspectiveCamera> camera_;
    vec3 cameraPos_;
    vec2u size_;

    struct {
      mat4 cubeModel;
      mat4 cubeView;
      mat4 cubeProj;
    } uniforms_;
    std::shared_ptr<gpu::Buffer> stateUbo_;

    std::shared_ptr<gpu::Framebuffer> fb_;

    struct {
      vec4 instancePositions[INSTANCES];
      std::shared_ptr<gpu::Pipeline> material;
      std::shared_ptr<gpu::Buffer> vertexBuff;
      std::shared_ptr<gpu::Buffer> indexBuff;
      std::shared_ptr<gpu::Buffer> instanceBuffer;
      std::shared_ptr<gpu::Texture> texture;
    } cube_;

    struct {
      std::shared_ptr<gpu::Pipeline> material;
      std::shared_ptr<gpu::Buffer> vertexBuff;
      std::shared_ptr<gpu::Buffer> indexBuff;
      Transform transform;
    } quad_;

    void *texData_ = nullptr;
    int32_t instances_ = INSTANCES / 8;

    bool quadRotation_ = true;
  };

}

#endif //XE_TEST_LAYER_HPP
