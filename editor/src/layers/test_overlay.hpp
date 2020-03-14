//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_TEST_OVERLAY_HPP
#define XE_TEST_OVERLAY_HPP

#include <xe/core/engine.hpp>

struct Quad {
  xe::gpu::Pipeline material;
  xe::gpu::Buffer vertexBuff;
  xe::gpu::Buffer indexBuff;
  xe::gpu::Texture texture;
  xe::mat4 proj;
  xe::mat4 view;
  xe::mat4 model;
};

struct Rotation {
  float angle;
};

struct Renderer {

};

class TestOverlay {
public:
  explicit TestOverlay();

  void start();
  void render();
  void update(xe::Timestep ts);

  bool onKeyPressed(const xe::Event::Key &e);

private:
  void *texData_ = nullptr;
};

#endif //XE_TEST_OVERLAY_HPP
