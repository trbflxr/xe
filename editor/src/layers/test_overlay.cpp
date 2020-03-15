//
// Created by FLXR on 9/11/2019.
//

#include "test_overlay.hpp"

#include <xe/utils/logger.hpp>
#include <xe/math/transform.hpp>
#include <xe/graphics/gpu_resources.hpp>

using namespace xe;

namespace quad1 {

  float vertexData[] = {
      -1.0, -1.0, 0.0, 0.0, 0.0,
      1.0, -1.0, 0.0, 1.0, 0.0,
      1.0, 1.0, 0.0, 1.0, 1.0,
      -1.0, 1.0, 0.0, 0.0, 1.0
  };
  uint16_t indexData[] = {0, 2, 1, 0, 3, 2};

  static const char *ecs_quad_vert = R"(
    #version 460 core

    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec2 a_texCoords;

    out vec2 v_texCoords;

    layout(location = 0) uniform mat4 u_proj;
    layout(location = 1) uniform mat4 u_view;
    layout(location = 2) uniform mat4 u_model;

    void main() {
      gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
      v_texCoords = a_texCoords;
    }
  )";

  static const char *ecs_quad_frag = R"(
    #version 460 core

    layout(location = 0) out vec4 color;

    in vec2 v_texCoords;

    uniform sampler2D u_tex2d0;

    void main() {
      color = texture(u_tex2d0, v_texCoords);
    }
  )";

}

TestOverlay::TestOverlay(Camera *camera) :
    camera_(camera) {

}

void TestOverlay::start() {
  Quad quad;
  quad.vertexBuff = Engine::ref().gpu().createBuffer(
      {BufferType::Vertex, Usage::Static, sizeof(quad1::vertexData)});
  quad.indexBuff = Engine::ref().gpu().createBuffer(
      {BufferType::Index, Usage::Static, sizeof(quad1::indexData)});


  gpu::Pipeline::Info::Shader quadShader;
  quadShader.vert = quad1::ecs_quad_vert;
  quadShader.frag = quad1::ecs_quad_frag;

  gpu::Pipeline::Info matInfo;
  matInfo.shader = quadShader;

  matInfo.attribs[0] = {"a_position", VertexFormat::Float3};
  matInfo.attribs[1] = {"a_texCoords", VertexFormat::Float2};

  matInfo.textures[0] = TextureType::T2D;
  matInfo.cull = Cull::Disabled;
  matInfo.blend.enabled = true;

  gpu::Texture::Info texInfo;
  texData_ = gpu::Texture::loadFromFile("textures/test.png", texInfo);
  texInfo.magFilter = TextureMagFilter::Linear;
  texInfo.minFilter = TextureMinFilter::LinearMipmapLinear;
  quad.texture = Engine::ref().gpu().createTexture(texInfo);

  quad.material = Engine::ref().gpu().createPipeline(matInfo);

  DisplayList frame;
  frame.fillBufferCommand()
      .set_buffer(quad.vertexBuff)
      .set_data(quad1::vertexData)
      .set_size(sizeof(quad1::vertexData));
  frame.fillBufferCommand()
      .set_buffer(quad.indexBuff)
      .set_data(quad1::indexData)
      .set_size(sizeof(quad1::indexData));
  frame.fillTextureCommand()
      .set_texture(quad.texture)
      .set_data0(texData_)
      .set_buildMipmap(true);
  Engine::ref().submitDrawList(std::move(frame));

  auto entity = Engine::ref().registry().create();
  Engine::ref().registry().assign<Quad>(entity, quad);
  Engine::ref().registry().assign<Renderer>(entity);
  Engine::ref().registry().assign<Transform>(entity);
}

void TestOverlay::render() {
  DisplayList frame;

  Engine::ref().registry().view<Quad, Transform>().each([&frame, this](auto &quad, auto &transform) {
    frame.setupViewCommand()
        .set_viewport({0, 0, camera_->viewport().x, camera_->viewport().y})
        .set_framebuffer(camera_->composer().framebuffer())
        .set_colorAttachment(0, true);
    frame.clearCommand()
        .set_color(Color::Clear)
        .set_clearColor(false)
        .set_clearDepth(true);
    frame.setupPipelineCommand()
        .set_pipeline(quad.material)
        .set_buffer(0, quad.vertexBuff)
        .set_uniform(0, {"u_proj", &camera_->projection(), sizeof(mat4)})
        .set_uniform(1, {"u_view", &camera_->view(), sizeof(mat4)})
        .set_uniform(2, {"u_model", &transform.worldTransform(), sizeof(mat4)})
        .set_texture(0, quad.texture);
    frame.renderCommand()
        .set_indexBuffer(quad.indexBuff)
        .set_count(sizeof(quad1::indexData) / sizeof(uint16_t))
        .set_type(IndexFormat::Uint16);
  });

  Engine::ref().submitDrawList(std::move(frame));
}

void TestOverlay::update(Timestep ts) {
  Engine::ref().registry().view<Quad, Transform>().each([ts](auto &quad, auto &transform) {
    transform.rotateY(45.0f * ts);
    transform.setWorldScale(0.2f);
    transform.setLocalPositionZ(3.5f);
  });
}

bool TestOverlay::onKeyPressed(const Event::Key &e) {
  if (e.code == Keyboard::T) {
    static std::string title = std::string(Engine::ref().window().getTitle());
    title += std::to_string(e.code);
    Engine::ref().window().setTitle(title);
  }
  XE_INFO("[TestOverlay] key pressed ({})", e.code);
  return false;
}
