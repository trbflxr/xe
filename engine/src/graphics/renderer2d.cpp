//
// Created by trbflxr on 3/15/2020.
//

#include "xepch.hpp"
#include <xe/graphics/renderer2d.hpp>

#include <xe/core/engine.hpp>

namespace xe {

  Renderer2d::Renderer2d(Camera &camera) :
      camera_(camera) {
    init();
  }

  void Renderer2d::init() {
    verticesSize_ = maxInstances_ * 4;
    instancesSize_ = maxInstances_ * 6;
    verticesBufferSize_ = verticesSize_ * sizeof(VertexData);
    instancesBufferSize_ = instancesSize_ * sizeof(uint32_t);

    initArrays();
    initPipeline();
    initBuffers();
  }

  void Renderer2d::initArrays() {
    bufferData_ = std::make_unique<VertexData[]>(verticesSize_);
    indices_ = std::make_unique<uint32_t[]>(instancesSize_);

    buffer_ = &bufferData_[0];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < maxInstances_; i += 6) {
      indices_[i + 0] = offset + 0;
      indices_[i + 1] = offset + 2;
      indices_[i + 2] = offset + 1;
      indices_[i + 3] = offset + 0;
      indices_[i + 4] = offset + 3;
      indices_[i + 5] = offset + 2;
      offset += 4;
    }
  }

  void Renderer2d::initPipeline() {
    gpu::Pipeline::Info pipelineInfo;
    pipelineInfo.shader = *Engine::ref().assetManager().getShader("renderer2d");

    pipelineInfo.attribs[0] = {"a_position", VertexFormat::Float2};
    pipelineInfo.attribs[1] = {"a_texCoords", VertexFormat::Float2};
    pipelineInfo.attribs[2] = {"a_color", VertexFormat::Float4};

    pipelineInfo.textures[0] = TextureType::T2D;
    pipelineInfo.blend.enabled = true;
    pipelineInfo.cull = Cull::Disabled;

    pipeline_ = Engine::ref().gpu().createPipeline(pipelineInfo);
  }

  void Renderer2d::initBuffers() {
    vertexBuffer_ = Engine::ref().gpu().createBuffer({BufferType::Vertex, Usage::Dynamic, verticesBufferSize_});
    indexBuffer_ = Engine::ref().gpu().createBuffer({BufferType::Index, Usage::Static, instancesBufferSize_});
    uniformBuffer_ = Engine::ref().gpu().createBuffer({BufferType::Uniform, Usage::Dynamic, sizeof(cameraData_), "Camera2DUniform", 0});

    DisplayList commands;
    commands.fillBufferCommand()
        .set_buffer(*vertexBuffer_)
        .set_data(&bufferData_[0])
        .set_size(verticesBufferSize_);
    commands.fillBufferCommand()
        .set_buffer(*indexBuffer_)
        .set_data(&indices_[0])
        .set_size(instancesBufferSize_);
    commands.fillBufferCommand()
        .set_buffer(*uniformBuffer_)
        .set_data(&cameraData_)
        .set_size(sizeof(cameraData_));
    Engine::ref().executeOnGpu(std::move(commands));
  }

  void Renderer2d::submit(const vec2 &pos, const vec2 &size, Color color, const std::shared_ptr<Texture> &texture, const rect &textureArea01) {
    if (texture) {
      if (activeTexture_) {
        if (activeTexture_->textureId() != texture->textureId()) {
          activeTexture_ = texture;
        }
      } else {
        activeTexture_ = texture;
      }
    }

    buffer_->position = pos;
    buffer_->texCoords = {textureArea01.x, textureArea01.y};
    buffer_->color = color;
    buffer_++;

    buffer_->position = {pos.x + size.x, pos.y};
    buffer_->texCoords = {textureArea01.width, textureArea01.y};
    buffer_->color = color;
    buffer_++;

    buffer_->position = pos + size;
    buffer_->texCoords = {textureArea01.width, textureArea01.height};
    buffer_->color = color;
    buffer_++;

    buffer_->position = {pos.x, pos.y + size.y};
    buffer_->texCoords = {textureArea01.x, textureArea01.height};
    buffer_->color = color;
    buffer_++;

    verticesCount_ += 4;
    indicesCount_ += 6;
  }

  void Renderer2d::begin() {
//    if (dirty_) {
//      bufferData_.reset(nullptr);
//      indices_.reset(nullptr);
//      init();
//      dirty_ = false;
//    }

    cameraData_.view = camera_.view();
    cameraData_.proj = camera_.projection();

    DisplayList commands;
    commands.setupViewCommand()
        .set_viewport(camera_.viewport())
        .set_framebuffer(Engine::ref().composer().framebuffer())
        .set_colorAttachment(0, true);
    commands.clearCommand()
        .set_color(Color::Fuchsia)
        .set_clearColor(false)
        .set_clearDepth(true);
    commands.fillBufferCommand()
        .set_buffer(*uniformBuffer_)
        .set_data(&cameraData_)
        .set_size(sizeof(cameraData_));
    Engine::ref().executeOnGpu(std::move(commands));
  }

  void Renderer2d::end() {

  }

  void Renderer2d::flush() {
    DisplayList commands;
    commands.fillBufferCommand()
        .set_buffer(*vertexBuffer_)
        .set_data(&bufferData_[0])
        .set_size(sizeof(VertexData) * verticesCount_);

    auto &cmd = commands.setupPipelineCommand()
        .set_pipeline(*pipeline_)
        .set_buffer(0, *vertexBuffer_)
        .set_uniformBuffer(0, *uniformBuffer_);
    if (activeTexture_) {
      cmd.set_texture(0, activeTexture_->raw());
    }

    commands.renderCommand()
        .set_indexBuffer(*indexBuffer_)
        .set_count(indicesCount_)
        .set_type(IndexFormat::Uint32);
    Engine::ref().executeOnGpu(std::move(commands));

    indicesCount_ = 0;
    verticesCount_ = 0;
    buffer_ = &bufferData_[0];
  }

  void Renderer2d::setMaxInstances(uint32_t instances) {
    XE_ASSERT(false, "Unsupported");
//    maxInstances_ = instances;
//    dirty_ = true;
  }

}
