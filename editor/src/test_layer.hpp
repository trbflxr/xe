//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_TEST_LAYER_HPP
#define XE_TEST_LAYER_HPP

#include <xe/core/layer.hpp>

struct State {
  xe::gpu::Framebuffer fb;
  static constexpr uint INSTANCES = 50000;
  struct {
    xe::vec4 instancePositions[INSTANCES];
    xe::gpu::Pipeline material;
    xe::gpu::Buffer vertexBuff;
    xe::gpu::Buffer indexBuff;
    xe::gpu::Buffer instanceBuffer;
    xe::gpu::Texture texture;
    xe::mat4 model;
    xe::mat4 view;
    xe::mat4 proj;
  } cube;
  struct {
    xe::gpu::Pipeline material;
    xe::gpu::Buffer vertexBuff;
    xe::gpu::Buffer indexBuff;
    xe::mat4 model;
    xe::mat4 view;
    xe::mat4 proj;
  } quad;
};

class TestLayer : public xe::Layer {
public:
  explicit TestLayer(xe::Application &app);
  ~TestLayer() override;

  void onInit() override;
  void onStop() override;

  void onRender() override;
  void onUpdate(xe::Timestep ts) override;


private:
  State state;
  void *texData_;
};

#endif //XE_TEST_LAYER_HPP
