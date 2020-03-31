//
// Created by trbflxr on 3/29/2020.
//

#include <xe/ui/ui.hpp>
#include "model_layer.hpp"

namespace xe {

  static uint32_t vbSize = 0;
  static uint32_t ibSize = 0;

  void ModelLayer::onStart() {
    model_ = std::make_shared<Model>("assets/models/Suzanne/Suzanne.gltf");
//    model_ = std::make_shared<Model>("assets/models/monkey3.obj");

    camera_ = std::make_unique<PerspectiveCamera>(vec2(1280, 720), 70.0f, 1280.0f / 720.0f, 1.0f, 1000.0f);
    cameraPos_ = {0.0f, 0.0f, -1.0f};
    camera_->transform().setLocalPosition(cameraPos_);

    //some hardcode
    vbSize = (uint32_t) model_->meshes_[0].vertices.size() * (uint32_t) sizeof(model_->meshes_[0].vertices[0]);
    ibSize = (uint32_t) model_->meshes_[0].indices.size() * (uint32_t) sizeof(uint32_t);

    //create buffers
    modelData_.vertexBuffer = Engine::ref().gpu().createBuffer({BufferType::Vertex, Usage::Static, vbSize});
    modelData_.indexBuffer = Engine::ref().gpu().createBuffer({BufferType::Index, Usage::Static, ibSize});

    //create texture
    gpu::Texture::Info info;
    info.minFilter = TextureMinFilter::Linear;
    info.magFilter = TextureMagFilter::Linear;
//    info.bindless = true;

    modelData_.texture = std::make_shared<Texture>();
    modelData_.texture->setInfo(info);
    modelData_.texture->loadFromFile("assets/models/Suzanne/Suzanne_BaseColor.png", false);
    modelData_.texture->setup();

    //setup pipeline
    gpu::Pipeline::Info matInfo;
    matInfo.shader = *Engine::ref().assetManager().getShader("model");

    matInfo.attribs[0] = {"a_position", VertexFormat::Float3};
    matInfo.attribs[1] = {"a_normal", VertexFormat::Float3};
    matInfo.attribs[2] = {"a_tangent", VertexFormat::Float3};
    matInfo.attribs[3] = {"a_biTangent", VertexFormat::Float3};
    matInfo.attribs[4] = {"a_texCoords", VertexFormat::Float2};

    matInfo.textures[0] = TextureType::T2D;
    matInfo.blend.enabled = true;
    matInfo.cull = Cull::Back;
    modelData_.pipeline = Engine::ref().gpu().createPipeline(matInfo);

    //fill buffers
    DisplayList commands;
    commands.fillBufferCommand()
        .set_buffer(*modelData_.vertexBuffer)
        .set_data(model_->meshes_[0].vertices.data())
        .set_size(vbSize);
    commands.fillBufferCommand()
        .set_buffer(*modelData_.indexBuffer)
        .set_data(model_->meshes_[0].indices.data())
        .set_size(ibSize);
    Engine::ref().executeOnGpu(std::move(commands));
  }

  void ModelLayer::onStop() {
    camera_->destroy();
    modelData_.texture->destroy();

    Engine::ref().gpu().destroyResource(*modelData_.vertexBuffer);
    Engine::ref().gpu().destroyResource(*modelData_.indexBuffer);
    Engine::ref().gpu().destroyResource(*modelData_.pipeline);
  }

  void ModelLayer::onPreRender() {
    camera_->updateUniforms();
  }

  void ModelLayer::onRender() {
    DisplayList frame;
    frame.setupViewCommand()
        .set_viewport(camera_->viewport())
        .set_framebuffer(Engine::ref().composer().framebuffer())
        .set_colorAttachment(0, true);
    frame.clearCommand()
        .set_clearColor(false)
        .set_clearDepth(true);
    frame.setupPipelineCommand()
        .set_pipeline(*modelData_.pipeline)
        .set_buffer(0, *modelData_.vertexBuffer)
        .set_uniformBuffer(0, camera_->uniformBuffer())
        .set_texture(0, modelData_.texture->raw());
    frame.renderCommand()
        .set_indexBuffer(*modelData_.indexBuffer)
        .set_count((uint32_t) model_->meshes_[0].indices.size())
        .set_type(IndexFormat::Uint32);
    Engine::ref().executeOnGpu(std::move(frame));
  }

  void ModelLayer::onUpdate() {

  }

  bool ModelLayer::onKeyPressed(Event::Key key) {
    return false;
  }

  bool ModelLayer::onUi() {
    ImGui::Text("ModelLayer:");
    if (ImGui::SliderFloat3("Camera pos##ModelLayer", reinterpret_cast<float *>(&cameraPos_), -5.0f, 5.0f)) {
      camera_->transform().setLocalPosition(cameraPos_);
    }
    return false;
  }

}
