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
  void onClose() override;

  void onRender() override;
  void onUpdate(xe::Timestep ts) override;

  bool onKeyPressed(const xe::Event::Key &e) override;

private:
  static void uiFunc(void *data);

private:
  State state_;
  void *texData_;
  int32 instances_;

  std::shared_ptr<xe::GameObject> camera_;
};

#endif //XE_TEST_LAYER_HPP
