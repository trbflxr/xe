//
// Created by FLXR on 8/31/2019.
//

#include <xe/core/application.hpp>
#include <xe/utils/debug_trace.hpp>
#include <xe/utils/logger.hpp>
#include <xe/core/vfs.hpp>
#include <xe/core/filesystem.hpp>

using namespace xe;

namespace cube {
  float vertexData[] = {
      -1.0, -1.0, -1.0, 0.1, 0.1, 0.3, 1.0, 0.0, 0.0,
      1.0, -1.0, -1.0, 0.1, 0.1, 0.3, 1.0, 1.0, 0.0,
      1.0, 1.0, -1.0, 0.1, 0.1, 0.3, 1.0, 1.0, 1.0,
      -1.0, 1.0, -1.0, 0.1, 0.1, 0.3, 1.0, 0.0, 1.0,

      -1.0, -1.0, 1.0, 0.1, 0.1, 1.0, 1.0, 0.0, 0.0,
      1.0, -1.0, 1.0, 0.1, 0.1, 1.0, 1.0, 1.0, 0.0,
      1.0, 1.0, 1.0, 0.1, 0.1, 1.0, 1.0, 1.0, 1.0,
      -1.0, 1.0, 1.0, 0.1, 0.1, 1.0, 1.0, 0.0, 1.0,

      -1.0, -1.0, -1.0, 0.3, 0.1, 0.1, 1.0, 0.0, 0.0,
      -1.0, 1.0, -1.0, 0.3, 0.1, 0.1, 1.0, 1.0, 0.0,
      -1.0, 1.0, 1.0, 0.3, 0.1, 0.1, 1.0, 1.0, 1.0,
      -1.0, -1.0, 1.0, 0.3, 0.1, 0.1, 1.0, 0.0, 1.0,

      1.0, -1.0, -1.0, 1.0, 0.1, 0.1, 1.0, 0.0, 0.0,
      1.0, 1.0, -1.0, 1.0, 0.1, 0.1, 1.0, 1.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 0.1, 0.1, 1.0, 1.0, 1.0,
      1.0, -1.0, 1.0, 1.0, 0.1, 0.1, 1.0, 0.0, 1.0,

      -1.0, -1.0, -1.0, 0.1, 0.3, 0.1, 1.0, 0.0, 0.0,
      -1.0, -1.0, 1.0, 0.1, 0.3, 0.1, 1.0, 1.0, 0.0,
      1.0, -1.0, 1.0, 0.1, 0.3, 0.1, 1.0, 1.0, 1.0,
      1.0, -1.0, -1.0, 0.1, 0.3, 0.1, 1.0, 0.0, 1.0,

      -1.0, 1.0, -1.0, 0.1, 1.0, 0.1, 1.0, 0.0, 0.0,
      -1.0, 1.0, 1.0, 0.1, 1.0, 0.1, 1.0, 1.0, 0.0,
      1.0, 1.0, 1.0, 0.1, 1.0, 0.1, 1.0, 1.0, 1.0,
      1.0, 1.0, -1.0, 0.1, 1.0, 0.1, 1.0, 0.0, 1.0
  };

  uint16 indexData[] = {
      0, 2, 1, 0, 3, 2, 6, 4, 5, 7, 4, 6,
      8, 10, 9, 8, 11, 10, 14, 12, 13, 15, 12, 14,
      16, 18, 17, 16, 19, 18, 22, 20, 21, 23, 20, 22
  };
}

namespace quad {
  float vertexData[] = {
      -1.0, -1.0, 0.0, 0.0, 0.0,
      1.0, -1.0, 0.0, 1.0, 0.0,
      1.0, 1.0, 0.0, 1.0, 1.0,
      -1.0, 1.0, 0.0, 0.0, 1.0
  };
  uint16 indexData[] = {0, 2, 1, 0, 3, 2};
}

struct State {
  struct UniformState {
    mat4 model;
    mat4 view;
    mat4 proj;
  };

  gpu::Framebuffer fb;
  static constexpr uint INSTANCES = 50000;
  struct {
    vec4 instancePositions[INSTANCES];
    gpu::Pipeline material;
    gpu::Buffer vertexBuff;
    gpu::Buffer indexBuff;
    gpu::Buffer instanceBuffer;
    gpu::Buffer uniformBuff;
    UniformState uniforms;
    gpu::Texture texture;
  } cube;
  struct {
    gpu::Pipeline material;
    gpu::Buffer vertexBuff;
    gpu::Buffer indexBuff;
    gpu::Buffer uniformBuff;
    UniformState uniforms;
  } quad;
};

class Editor : public Application {
XE_OBJECT(Editor, Application);
public:
  Editor() {
    xe::Logger::setLogLevel(LogLevel::Info, LogLevel::Info);

    Params params = {{800, 600,
                                "test",
                                    true,
                         0, 0, nullptr},
                     {128, 128, 64, 128}};

    Engine::ref().setParams(params);
  }

protected:
  void init() override {
    VFS::mount("shaders", "assets/shaders/");
    VFS::mount("textures", "assets/textures/");

    state.cube.uniforms.proj = mat4::perspective(60.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
    state.cube.uniforms.view = mat4::transformation({-20, 25, 60},
                                                    quat(vec3::unitY(), 90.0f) * quat(vec3::unitX(), 30.0f)).inverse();


    state.quad.uniforms.proj = mat4::perspective(60.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
    state.quad.uniforms.view = mat4::translation({0.0f, 0.0f, 2.0f}).inverse();
  }

  void start() override {
    state.cube.vertexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Vertex, Usage::Static, sizeof(cube::vertexData)});
    state.cube.indexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Index, Usage::Static, sizeof(cube::indexData)});
    state.cube.instanceBuffer = Engine::ref().gpu().createBuffer(
        {BufferType::Vertex, Usage::Dynamic, sizeof(state.cube.instancePositions)});
    state.cube.uniformBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Uniform, Usage::Static, sizeof(State::UniformState), "UniformState"});

    state.quad.vertexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Vertex, Usage::Static, sizeof(quad::vertexData)});
    state.quad.indexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Index, Usage::Static, sizeof(quad::indexData)});
    state.quad.uniformBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Uniform, Usage::Dynamic, sizeof(State::UniformState), "UniformState0"});

    {
      gpu::Pipeline::Info matInfo;
      VFS::readTextFile("/shaders/test.vert", matInfo.shader.vert);
      VFS::readTextFile("/shaders/test.frag", matInfo.shader.frag);

      matInfo.attribs[0] = {"a_position", VertexFormat::Float3};
      matInfo.attribs[1] = {"a_color", VertexFormat::Float4};
      matInfo.attribs[2] = {"a_texCoords", VertexFormat::Float2};
      matInfo.attribs[3] = {"a_instancePosition", VertexFormat::Float4, 1, VertexStep::PerInstance};

      matInfo.textures[0] = TextureType::T2D;

      gpu::Texture::Info texInfo;
      texData_ = gpu::Texture::loadFromFile("assets/textures/test.png", texInfo);
      texInfo.magFilter = TextureMagFilter::Linear;
      texInfo.minFilter = TextureMinFilter::LinearMipmapLinear;
      state.cube.texture = Engine::ref().gpu().createTexture(texInfo);

      state.cube.material = Engine::ref().gpu().createPipeline(matInfo);
      {
        DisplayList frame;
        frame.fillBufferCommand()
            .set_buffer(state.cube.vertexBuff)
            .set_data(cube::vertexData)
            .set_size(sizeof(cube::vertexData));
        frame.fillBufferCommand()
            .set_buffer(state.cube.indexBuff)
            .set_data(cube::indexData)
            .set_size(sizeof(cube::indexData));
        frame.fillBufferCommand()
            .set_buffer(state.cube.uniformBuff)
            .set_data(&state.cube.uniforms)
            .set_size(sizeof(State::UniformState));
        frame.fillTextureCommand()
            .set_texture(state.cube.texture)
            .set_data0(texData_)
            .set_buildMipmap(true);
        Engine::ref().submitDrawList(std::move(frame));
      }
    }

    {
      gpu::Pipeline::Info matInfo;
      VFS::readTextFile("/shaders/fb_test.vert", matInfo.shader.vert);
      VFS::readTextFile("/shaders/fb_test.frag", matInfo.shader.frag);

      matInfo.attribs[0] = {"a_position", VertexFormat::Float3};
      matInfo.attribs[1] = {"a_texCoords", VertexFormat::Float2};

      matInfo.textures[0] = TextureType::T2D;
      matInfo.textures[1] = TextureType::T2D;
      matInfo.cull = Cull::Disabled;
      state.quad.material = Engine::ref().gpu().createPipeline(matInfo);

      {
        DisplayList frame;
        frame.fillBufferCommand()
            .set_buffer(state.quad.vertexBuff)
            .set_data(quad::vertexData)
            .set_size(sizeof(quad::vertexData));
        frame.fillBufferCommand()
            .set_buffer(state.quad.indexBuff)
            .set_data(quad::indexData)
            .set_size(sizeof(quad::indexData));
        frame.fillBufferCommand()
            .set_buffer(state.quad.uniformBuff)
            .set_data(&state.quad.uniforms)
            .set_size(sizeof(State::UniformState));
        Engine::ref().submitDrawList(std::move(frame));
      }

      gpu::Texture::Info fbColor0 = {800, 600};
      fbColor0.format = TexelsFormat::Rgba8;

      gpu::Texture::Info fbColor1 = {800, 600};
      fbColor1.format = TexelsFormat::Rgba8;

      gpu::Texture::Info fbDepth = {800, 600};
      fbDepth.format = TexelsFormat::Depth16;

      gpu::Framebuffer::Info fbInfo;
      fbInfo.colorAttachmentInfo[0] = fbColor0;
      fbInfo.colorAttachmentInfo[1] = fbColor1;
      fbInfo.depthStencilAttachmentInfo = fbDepth;
      fbInfo.colorAttachmentsSize = 2;

      state.fb = Engine::ref().gpu().createFramebuffer(fbInfo);
    }
  }

  void update(Timestep ts) override {
    static float v = 0;
    for (size_t i = 0; i < State::INSTANCES; ++i) {
      state.cube.instancePositions[i] = {
          (i / 100) * 2.0f,
          750.0f * (float) (sin(i * XE_MATH_PI / 10.0f + v) * ts),
          (float) (i % 100) * 1.2f,
          0.0f
      };
    }
    v += 0.01f;

    static float angle = 0.0f;
    angle += 45.0f * ts;

    state.quad.uniforms.model = mat4::transformation(vec3(), {{0, 1, 0}, angle});
  }

  void renderUpdate() override {
    DisplayList frame;
    frame.setupViewCommand()
        .set_viewport({0, 0, 800, 600})
        .set_projectionMatrix(state.cube.uniforms.proj)
        .set_viewMatrix(state.cube.uniforms.view)
        .set_framebuffer(state.fb)
        .set_colorAttachment(0, true)
        .set_colorAttachment(1, true);
    frame.clearCommand()
        .set_color(Color(0xFFA6A6A6))
        .set_clearColor(true)
        .set_clearDepth(true);
    frame.fillBufferCommand()
        .set_buffer(state.cube.instanceBuffer)
        .set_data(state.cube.instancePositions)
        .set_size(sizeof(state.cube.instancePositions));
    frame.setupPipelineCommand()
        .set_pipeline(state.cube.material)
        .set_buffer(0, state.cube.vertexBuff)
        .set_buffer(1, state.cube.instanceBuffer)
        .set_modelMatrix(state.cube.uniforms.view)
        .set_uniformBuffer(0, state.cube.uniformBuff)
        .set_texture(0, state.cube.texture);
    frame.renderCommand()
        .set_indexBuffer(state.cube.indexBuff)
        .set_count(sizeof(cube::indexData) / sizeof(uint16))
        .set_type(IndexFormat::Uint16)
        .set_instances(State::INSTANCES);

    //framebuffer
    frame.setupViewCommand()
        .set_viewport({0, 0, 800, 600})
        .set_projectionMatrix(state.quad.uniforms.proj)
        .set_viewMatrix(state.quad.uniforms.proj);
    frame.clearCommand()
        .set_color(Color(Color::yellow()))
        .set_clearColor(true)
        .set_clearDepth(true);
    frame.fillBufferCommand()
        .set_buffer(state.quad.uniformBuff)
        .set_data(&state.quad.uniforms)
        .set_size(sizeof(State::UniformState));
    frame.setupPipelineCommand()
        .set_pipeline(state.quad.material)
        .set_buffer(0, state.quad.vertexBuff)
        .set_modelMatrix(state.quad.uniforms.model)
        .set_uniformBuffer(0, state.quad.uniformBuff)
        .set_texture(0, state.fb.colorAttachment(0))
        .set_texture(1, state.fb.colorAttachment(1));
    frame.renderCommand()
        .set_indexBuffer(state.quad.indexBuff)
        .set_count(sizeof(quad::indexData) / sizeof(uint16))
        .set_type(IndexFormat::Uint16);

    Engine::ref().submitDrawList(std::move(frame));
  }

private:
  State state;
  void *texData_;
};

XE_DEFINE_ENTRY_POINT(Editor);
