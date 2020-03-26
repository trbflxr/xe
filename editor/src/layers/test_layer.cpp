//
// Created by FLXR on 9/11/2019.
//

#include "test_layer.hpp"

#include <xe/core/vfs.hpp>
#include <xe/utils/logger.hpp>
#include <xe/ui/ui.hpp>

namespace cube {
  float vertexData[] = {
      -1.0, -1.0, 1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      1.0, -1.0, 1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      1.0, 1.0, 1.0, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      -1.0, 1.0, 1.0, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

      -1.0, -1.0, -1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      1.0, -1.0, -1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      1.0, 1.0, -1.0, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      -1.0, 1.0, -1.0, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
  };

  uint16_t indexData[] = {
      0, 1, 2,
      2, 3, 0,

      1, 5, 6,
      6, 2, 1,

      7, 6, 5,
      5, 4, 7,

      4, 0, 3,
      3, 7, 4,

      4, 5, 1,
      1, 0, 4,

      3, 2, 6,
      6, 7, 3
  };
}

namespace quad {
  float vertexData[] = {
      -1.0, -1.0, 0.0, 0.0, 0.0,
      1.0, -1.0, 0.0, 1.0, 0.0,
      1.0, 1.0, 0.0, 1.0, 1.0,
      -1.0, 1.0, 0.0, 0.0, 1.0
  };
  uint16_t indexData[] = {0, 2, 1, 0, 3, 2};
}

namespace xe {

  TestLayer::TestLayer() :
      size_(Engine::ref().getParams().window.width, Engine::ref().getParams().window.height) {

    camera_ = std::make_unique<PerspectiveCamera>(size_, 70.0f, static_cast<float>(size_.x) / size_.y, 1.0f, 1000.0f);
  }

  void TestLayer::onStart() {
    uniforms_.cubeProj = mat4::perspective(60.0f, static_cast<float>(size_.x) / size_.y, 1.0f, 1000.0f);
    uniforms_.cubeView = mat4::transformation({-10, 25, 60},
                                              quat(vec3::unitY(), 90.0f) * quat(vec3::unitX(), 30.0f)).inverse();

    quad_.transform.setLocalPositionZ(2.0f);

    cube_.vertexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Vertex, Usage::Static, sizeof(cube::vertexData)});
    cube_.indexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Index, Usage::Static, sizeof(cube::indexData)});
    cube_.instanceBuffer = Engine::ref().gpu().createBuffer(
        {BufferType::Vertex, Usage::Dynamic, sizeof(cube_.instancePositions)});

    quad_.vertexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Vertex, Usage::Static, sizeof(quad::vertexData)});
    quad_.indexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Index, Usage::Static, sizeof(quad::indexData)});
    stateUbo_ = Engine::ref().gpu().createBuffer(
        {BufferType::Uniform, Usage::Dynamic, sizeof(uniforms_), "StateUniform", 3});

    {
      gpu::Pipeline::Info matInfo;
      matInfo.shader = *Engine::ref().assetManager().getShader("test");

      matInfo.attribs[0] = {"a_position", VertexFormat::Float3};
      matInfo.attribs[1] = {"a_texCoords", VertexFormat::Float2};
      matInfo.attribs[2] = {"a_color", VertexFormat::Float4};
      matInfo.attribs[3] = {"a_instancePosition", VertexFormat::Float4, 1, VertexStep::PerInstance};

      matInfo.textures[0] = TextureType::T2D;

      matInfo.blend.enabled = true;

      gpu::Texture::Info texInfo;
      texData_ = gpu::Texture::loadFromFile("textures/test.png", texInfo);
      texInfo.magFilter = TextureMagFilter::Linear;
      texInfo.minFilter = TextureMinFilter::LinearMipmapLinear;
      cube_.texture = Engine::ref().gpu().createTexture(texInfo);

      {
        //fill uniforms
        DisplayList frame;
        frame.fillBufferCommand()
            .set_buffer(*stateUbo_)
            .set_data(&uniforms_)
            .set_size(sizeof(uniforms_));
        Engine::ref().executeOnGpu(std::move(frame));
      }

      cube_.material = Engine::ref().gpu().createPipeline(matInfo);
      {
        DisplayList frame;
        frame.fillBufferCommand()
            .set_buffer(*cube_.vertexBuff)
            .set_data(cube::vertexData)
            .set_size(sizeof(cube::vertexData));
        frame.fillBufferCommand()
            .set_buffer(*cube_.indexBuff)
            .set_data(cube::indexData)
            .set_size(sizeof(cube::indexData));
        frame.fillTextureCommand()
            .set_texture(*cube_.texture)
            .set_data0(texData_)
            .set_buildMipmap(true);
        Engine::ref().executeOnGpu(std::move(frame));
      }
    }

    {
      gpu::Pipeline::Info matInfo;
      matInfo.shader = *Engine::ref().assetManager().getShader("fb_test");

      matInfo.attribs[0] = {"a_position", VertexFormat::Float3};
      matInfo.attribs[1] = {"a_texCoords", VertexFormat::Float2};

      matInfo.textures[0] = TextureType::T2D;
      matInfo.textures[1] = TextureType::T2D;
      matInfo.cull = Cull::Disabled;
      matInfo.blend.enabled = true;
      quad_.material = Engine::ref().gpu().createPipeline(matInfo);

      {
        DisplayList frame;
        frame.fillBufferCommand()
            .set_buffer(*quad_.vertexBuff)
            .set_data(quad::vertexData)
            .set_size(sizeof(quad::vertexData));
        frame.fillBufferCommand()
            .set_buffer(*quad_.indexBuff)
            .set_data(quad::indexData)
            .set_size(sizeof(quad::indexData));
        Engine::ref().executeOnGpu(std::move(frame));
      }

      gpu::Texture::Info fbColor0 = {size_.x, size_.y};
      fbColor0.format = TexelsFormat::Rgba8;

      gpu::Texture::Info fbColor1 = {size_.x, size_.y};
      fbColor1.format = TexelsFormat::Rgba8;

      gpu::Texture::Info fbDepth = {size_.x, size_.y};
      fbDepth.format = TexelsFormat::Depth16;

      gpu::Framebuffer::Info fbInfo;
      fbInfo.colorAttachmentInfo[0] = fbColor0;
      fbInfo.colorAttachmentInfo[1] = fbColor1;
      fbInfo.depthStencilAttachmentInfo = fbDepth;
      fbInfo.colorAttachmentsSize = 2;

      fb_ = Engine::ref().gpu().createFramebuffer(fbInfo);
    }
  }

  void TestLayer::onStop() {
    camera_->destroy();

    Engine::ref().gpu().destroyResource(*stateUbo_);
    Engine::ref().gpu().destroyResource(*fb_);

    Engine::ref().gpu().destroyResource(*cube_.material);
    Engine::ref().gpu().destroyResource(*cube_.vertexBuff);
    Engine::ref().gpu().destroyResource(*cube_.indexBuff);
    Engine::ref().gpu().destroyResource(*cube_.instanceBuffer);
    Engine::ref().gpu().destroyResource(*cube_.texture);

    Engine::ref().gpu().destroyResource(*quad_.material);
    Engine::ref().gpu().destroyResource(*quad_.vertexBuff);
    Engine::ref().gpu().destroyResource(*quad_.indexBuff);

    delete reinterpret_cast<uint8_t *>(texData_);
  }

  void TestLayer::onRender() {
    camera_->updateUniforms();

    DisplayList frame;
    frame.fillBufferCommand()
        .set_buffer(*stateUbo_)
        .set_data(&uniforms_)
        .set_size(sizeof(uniforms_));

    frame.setupViewCommand()
        .set_viewport({0, 0, size_.x, size_.y})
        .set_framebuffer(*fb_)
        .set_colorAttachment(0, true)
        .set_colorAttachment(1, true);
    frame.clearCommand()
        .set_color(Color(0xF4A1AA))
        .set_clearColor(true)
        .set_clearDepth(true);
    frame.fillBufferCommand()
        .set_buffer(*cube_.instanceBuffer)
        .set_data(cube_.instancePositions)
        .set_size(sizeof(cube_.instancePositions));
    frame.setupPipelineCommand()
        .set_pipeline(*cube_.material)
        .set_buffer(0, *cube_.vertexBuff)
        .set_buffer(1, *cube_.instanceBuffer)
        .set_uniformBuffer(0, *stateUbo_)
        .set_texture(0, *cube_.texture);
    frame.renderCommand()
        .set_indexBuffer(*cube_.indexBuff)
        .set_count(sizeof(cube::indexData) / sizeof(uint16_t))
        .set_type(IndexFormat::Uint16)
        .set_instances(instances_);

    Color tint = Color::Aqua;

    // framebuffer
    frame.setupViewCommand()
        .set_viewport(camera_->viewport())
        .set_framebuffer(Engine::ref().composer().framebuffer())
        .set_colorAttachment(0, true);
    frame.clearCommand()
        .set_color(Color::Clear)
        .set_clearColor(false)
        .set_clearDepth(true);
    frame.setupPipelineCommand()
        .set_pipeline(*quad_.material)
        .set_buffer(0, *quad_.vertexBuff)
        .set_uniformBuffer(0, camera_->uniformBuffer())
        .set_uniform(0, {"u_model", &quad_.transform.worldTransform(), sizeof(mat4)})
        .set_uniform(1, {"u_tint", &tint, sizeof(Color)})
        .set_texture(0, fb_->colorAttachment(0))
        .set_texture(1, fb_->colorAttachment(1));
    frame.renderCommand()
        .set_indexBuffer(*quad_.indexBuff)
        .set_count(sizeof(quad::indexData) / sizeof(uint16_t))
        .set_type(IndexFormat::Uint16);

    Engine::ref().executeOnGpu(std::move(frame));
  }

  void TestLayer::onUpdate() {
    auto ts = Engine::ref().delta();

    static float v = 0;
    for (int32_t i = 0; i < instances_; ++i) {
      cube_.instancePositions[i] = {
          (i / 100.0f) * 2.0f,
          750.0f * sin(i * math::Pi / 10.0f + v) * ts,
          static_cast<float>(i % 100) * 1.2f,
          0.0f
      };
    }
    v += 0.01f;

    static float angle = 0.0f;
    angle += 45.0f * ts;

    if (quadRotation_) {
      quad_.transform.rotateY(15.0f * ts);
    }

    quad_.transform.setLocalPositionZ(-2.5f);
  }

  bool TestLayer::onKeyPressed(Event::Key e) {
//  XE_INFO("[TestLayer] key pressed ({})", e.code);
    return false;
  }

  bool TestLayer::onUi() {
    ImGui::Text("TestLayer:");
    ImGui::SliderInt("Instances", &instances_, 1, INSTANCES);
    if (ImGui::Button("Reset rotation")) {
      quad_.transform.setLocalRotation({0.0f, 0.0f, 0.0f});
    }
    ImGui::SameLine();
    ImGui::Checkbox("Quad rotation", &quadRotation_);
    if (ImGui::SliderFloat3("Camera pos##TestLayer", reinterpret_cast<float *>(&cameraPos_), -5.0f, 5.0f)) {
      camera_->transform().setLocalPositionX(cameraPos_[0]);
      camera_->transform().setLocalPositionY(cameraPos_[1]);
      camera_->transform().setLocalPositionZ(cameraPos_[2]);
    }
    return false;
  }

}
