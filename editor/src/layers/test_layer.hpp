//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_TEST_LAYER_HPP
#define XE_TEST_LAYER_HPP

#include <xe/core/engine.hpp>

struct State {
  xe::gpu::Framebuffer fb;
  static constexpr uint32_t INSTANCES = 50000;
  struct {
    xe::vec4 instancePositions[INSTANCES];
    xe::gpu::Pipeline material;
    xe::gpu::Buffer vertexBuff;
    xe::gpu::Buffer indexBuff;
    xe::gpu::Buffer instanceBuffer;
    xe::gpu::Texture texture;
  } cube;
  struct {
    xe::gpu::Pipeline material;
    xe::gpu::Buffer vertexBuff;
    xe::gpu::Buffer indexBuff;
  } quad;

  xe::gpu::Buffer stateUbo;
  struct {
    xe::mat4 cubeModel;
    xe::mat4 cubeView;
    xe::mat4 cubeProj;

    xe::mat4 quadModel;
    xe::mat4 quadView;
    xe::mat4 quadProj;
  } uniforms;
};

class TestLayer {
public:
  explicit TestLayer();
  ~TestLayer();

  void start();
  void stop();

  void render();
  void update(xe::Timestep ts);

  bool onKeyPressed(const xe::Event::Key &e);

private:
  static void uiFunc(void *data);

private:
  State state_;
  void *texData_;
  int32_t instances_;
};

#endif //XE_TEST_LAYER_HPP
