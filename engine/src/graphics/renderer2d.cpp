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


    //todo: remove
    gpu::Texture::Info texInfo;
    void *texData = gpu::Texture::loadFromFile("textures/test.png", texInfo);
    texInfo.magFilter = TextureMagFilter::Linear;
    texInfo.minFilter = TextureMinFilter::LinearMipmapLinear;
    activeTexture_ = Engine::ref().gpu().createTexture(texInfo);


    DisplayList executeOnGpu;
    executeOnGpu.fillTextureCommand()
        .set_texture(activeTexture_)
        .set_data0(texData)
        .set_buildMipmap(true);
    Engine::ref().submitDrawList(std::move(executeOnGpu));
  }

  void Renderer2d::initBuffers() {
    vertexBuffer_ = Engine::ref().gpu().createBuffer({BufferType::Vertex, Usage::Dynamic, verticesBufferSize_});
    indexBuffer_ = Engine::ref().gpu().createBuffer({BufferType::Index, Usage::Static, instancesBufferSize_});

    DisplayList executeOnGpu;
    executeOnGpu.fillBufferCommand()
        .set_buffer(vertexBuffer_)
        .set_data(&bufferData_[0])
        .set_size(verticesBufferSize_);
    executeOnGpu.fillBufferCommand()
        .set_buffer(indexBuffer_)
        .set_data(&indices_[0])
        .set_size(instancesBufferSize_);
    Engine::ref().submitDrawList(std::move(executeOnGpu));
  }

  void Renderer2d::submit(const vec2 &pos, const vec2 &size, Color color) {
    buffer_->position = pos;
    buffer_->texCoords = {0.0f, 0.0f};
    buffer_->color = color;
    buffer_++;

    buffer_->position = {pos.x + size.x, pos.y};
    buffer_->texCoords = {1.0f, 0.0f};
    buffer_->color = color;
    buffer_++;

    buffer_->position = pos + size;
    buffer_->texCoords = {1.0f, 1.0f};
    buffer_->color = color;
    buffer_++;

    buffer_->position = {pos.x, pos.y + size.y};
    buffer_->texCoords = {0.0f, 1.0f};
    buffer_->color = color;
    buffer_++;

    verticesCount_ += 4;
    indicesCount_ += 6;
  }

  void Renderer2d::begin() {
    DisplayList executeOnGpu;
    executeOnGpu.setupViewCommand()
        .set_viewport(camera_.viewport())
        .set_framebuffer(Engine::ref().composer().framebuffer())
        .set_colorAttachment(0, true);
    executeOnGpu.clearCommand()
        .set_color(Color::Fuchsia)
        .set_clearColor(false)
        .set_clearDepth(true);
    Engine::ref().submitDrawList(std::move(executeOnGpu));
  }

  void Renderer2d::end() {

  }

  void Renderer2d::flush() {
    DisplayList executeOnGpu;
    executeOnGpu.fillBufferCommand()
        .set_buffer(vertexBuffer_)
        .set_data(&bufferData_[0])
        .set_size(sizeof(VertexData) * verticesCount_);

    executeOnGpu.setupPipelineCommand()
        .set_pipeline(pipeline_)
        .set_buffer(0, vertexBuffer_)
        .set_uniform(0, {"u_proj", &camera_.projection(), sizeof(mat4)})
        .set_uniform(1, {"u_view", &camera_.view(), sizeof(mat4)})
        .set_texture(0, activeTexture_);
    executeOnGpu.renderCommand()
        .set_indexBuffer(indexBuffer_)
        .set_count(indicesCount_)
        .set_type(IndexFormat::Uint32);
    Engine::ref().submitDrawList(std::move(executeOnGpu));

    indicesCount_ = 0;
    verticesCount_ = 0;
    buffer_ = &bufferData_[0];
  }

}
