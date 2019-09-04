//
// Created by FLXR on 8/31/2019.
//

#include <xe/core/application.hpp>
#include <xe/utils/debug_trace.hpp>
#include <xe/utils/logger.hpp>

using namespace xe;

class Editor : public Application {
XE_OBJECT(Editor, Application);
public:
  Editor() {
    Params params = {{800, 600,
                                "test",
                                     true,
                         0, 0, nullptr},
                     {128, 128, 128, 128}};

    Engine::ref().setParams(params);
  }

protected:
  void init() override {

  }

  void start() override {
    static float vertexData[] = {
        -0.5, -0.5, 0.0, 1.0, 0.0, 0.0,
        0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.5, 0.0, 0.0, 0.0, 1.0,
    };

    static uint16 indexData[] = {
        0, 1, 2,
    };

    vbo_ = Engine::ref().gpu().createBuffer({BufferType::Vertex, Usage::Static, sizeof(vertexData)});
    ibo_ = Engine::ref().gpu().createBuffer({BufferType::Index, Usage::Static, sizeof(indexData)});

    DrawList frame;
    frame.fillBufferCommand()
        .set_buffer(vbo_)
        .set_data(vertexData)
        .set_size(sizeof(vertexData));
    frame.fillBufferCommand()
        .set_buffer(ibo_)
        .set_data(indexData)
        .set_size(sizeof(indexData));
    Engine::ref().submitDrawList(std::move(frame));
  }

  void renderUpdate() override {
    DrawList frame;
    frame.clearCommand()
        .set_color({1, 0, 1, 1});
    frame.renderCommand()
        .set_indexBuffer(ibo_)
        .set_count(1)
        .set_type(IndexFormat::Uint16);
    Engine::ref().submitDrawList(std::move(frame));
  }

private:
  gpu::Buffer vbo_;
  gpu::Buffer ibo_;

};

XE_DEFINE_ENTRY_POINT(Editor);
