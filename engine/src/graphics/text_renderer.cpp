//
// Created by trbflxr on 3/26/2020.
//

#include "xepch.hpp"
#include <xe/graphics/text_renderer.hpp>

#include <xe/core/engine.hpp>

namespace xe {

  TextRenderer::TextRenderer(uint32_t screenWidth, uint32_t screenHeight, uint32_t maxInstances) :
      resolution_(screenWidth, screenHeight),
      maxInstances_(maxInstances) {
    setName("TextRenderer");
    init();
  }

  void TextRenderer::destroy() {
    camera_->destroy();
    Engine::ref().gpu().destroyResource(*pipeline_);
    Engine::ref().gpu().destroyResource(*vertexBuffer_);
    Engine::ref().gpu().destroyResource(*indexBuffer_);
    Engine::ref().gpu().destroyResource(*uniformBuffer_);
  }

  void TextRenderer::init() {
    camera_ = std::make_unique<OrthographicCamera>(resolution_,
                                                   0.0f, static_cast<float>(resolution_.x),
                                                   0.0f, static_cast<float>(resolution_.y),
                                                   -1.0f, 1.0f);

    verticesSize_ = maxInstances_ * 4;
    indicesSize_ = maxInstances_ * 6;
    verticesBufferSize_ = verticesSize_ * sizeof(VertexData);
    indicesBufferSize_ = indicesSize_ * sizeof(uint32_t);

    initArrays();
    initPipeline();
    initBuffers();
    initUniforms();
  }

  void TextRenderer::initArrays() {
    bufferData_ = std::make_unique<VertexData[]>(verticesSize_);
    indices_ = std::make_unique<uint32_t[]>(indicesSize_);

    buffer_ = &bufferData_[0];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < indicesSize_; i += 6) {
      indices_[i + 0] = offset + 0;
      indices_[i + 1] = offset + 2;
      indices_[i + 2] = offset + 1;
      indices_[i + 3] = offset + 0;
      indices_[i + 4] = offset + 3;
      indices_[i + 5] = offset + 2;
      offset += 4;
    }
  }

  void TextRenderer::initPipeline() {
    gpu::Pipeline::Info pipelineInfo;
    pipelineInfo.shader = *Engine::ref().assetManager().getShader("text_renderer");

    pipelineInfo.attribs[0] = {"a_position", VertexFormat::Float2};
    pipelineInfo.attribs[1] = {"a_texCoords", VertexFormat::Float2};
    pipelineInfo.attribs[2] = {"a_color", VertexFormat::Float4};
    pipelineInfo.attribs[3] = {"a_outlineColor", VertexFormat::Float4};
    pipelineInfo.attribs[4] = {"a_texHandle", VertexFormat::Uint32_2};
    pipelineInfo.attribs[5] = {"a_fontData", VertexFormat::Float2};

    pipelineInfo.blend.enabled = true;
    pipelineInfo.cull = Cull::Disabled;

    pipeline_ = Engine::ref().gpu().createPipeline(pipelineInfo);
    if (!pipeline_) {
      XE_CORE_CRITICAL("[TextRenderer] Failed to create pipeline");
    }
  }

  void TextRenderer::initBuffers() {
    vertexBuffer_ = Engine::ref().gpu().createBuffer({BufferType::Vertex, Usage::Dynamic, verticesBufferSize_});
    if (!vertexBuffer_) {
      XE_CORE_CRITICAL("[TextRenderer] Failed to create vertex buffer");
    }

    indexBuffer_ = Engine::ref().gpu().createBuffer({BufferType::Index, Usage::Static, indicesBufferSize_});
    if (!indexBuffer_) {
      XE_CORE_CRITICAL("[TextRenderer] Failed to create index buffer");
    }

    DisplayList commands;
    commands.fillBufferCommand()
        .set_buffer(*vertexBuffer_)
        .set_data(&bufferData_[0])
        .set_size(verticesBufferSize_);
    commands.fillBufferCommand()
        .set_buffer(*indexBuffer_)
        .set_data(&indices_[0])
        .set_size(indicesBufferSize_);
    Engine::ref().executeOnGpu(std::move(commands));
  }

  void TextRenderer::initUniforms() {
    uniformBuffer_ = Engine::ref().gpu().createBuffer(
        {BufferType::Uniform, Usage::Dynamic, sizeof(cameraData_), "Camera2DUniform", cCamera2dUboBinding});
    if (!uniformBuffer_) {
      XE_CORE_CRITICAL("[TextRenderer] Failed to create uniform buffer");
    }

    DisplayList commands;
    commands.fillBufferCommand()
        .set_buffer(*uniformBuffer_)
        .set_data(&cameraData_)
        .set_size(sizeof(cameraData_));
    Engine::ref().executeOnGpu(std::move(commands));
  }

  void TextRenderer::submit(const Text &text) {
    if (verticesCount_ >= verticesSize_ || indicesCount_ >= indicesSize_) {
      XE_CORE_ERROR("[TextRenderer] Maximum sprites reached ({})", maxInstances_);
      return;
    }

    if (!text.font() || !text.font()->atlas()) {
      XE_CORE_ERROR("[TextRenderer] Unable to render text ({}). Font or font atlas == nullptr", text.string());
      return;
    }

    uint64_t handle = text.font()->atlas()->handle();
    const auto &str = text.string();
    const float scale = text.scale();
    const float fs = text.size() * scale;
    const float outlineWidth = text.outlineWidth();
    const float outlineEdge = text.outlineEdge();

    vec2 pos = text.position();

    Glyph g{ };
    Glyph gPrev{ };
    for (size_t i = 0; i < str.size(); ++i) {
      uint32_t cpSize = 0;
      const uint32_t cp = string::utf8ToUtf32(&str[i], &cpSize);

      i += cpSize - 1;

      gPrev = g;
      g = text.font()->getGlyph(cp);

      //kerning
      if (i > 0) {
        pos.x += text.font()->getKerning(gPrev.unicode, g.unicode);
      }

      buffer_->position = {pos.x + g.planeBounds.left * fs, pos.y + g.planeBounds.bottom * fs};
      buffer_->texCoords = {g.atlasBounds.left, g.atlasBounds.bottom};
      buffer_->color = text.color();
      buffer_->outlineColor = text.outlineColor();
      buffer_->texHandle = *reinterpret_cast<const vec2u *>(&handle);
      buffer_->data = {outlineWidth, outlineEdge};
      buffer_++;

      buffer_->position = {pos.x + g.planeBounds.right * fs, pos.y + g.planeBounds.bottom * fs};
      buffer_->texCoords = {g.atlasBounds.right, g.atlasBounds.bottom};
      buffer_->color = text.color();
      buffer_->outlineColor = text.outlineColor();
      buffer_->texHandle = *reinterpret_cast<const vec2u *>(&handle);
      buffer_->data = {outlineWidth, outlineEdge};
      buffer_++;

      buffer_->position = {pos.x + g.planeBounds.right * fs, pos.y + g.planeBounds.top * fs};
      buffer_->texCoords = {g.atlasBounds.right, g.atlasBounds.top};
      buffer_->color = text.color();
      buffer_->outlineColor = text.outlineColor();
      buffer_->texHandle = *reinterpret_cast<const vec2u *>(&handle);
      buffer_->data = {outlineWidth, outlineEdge};
      buffer_++;

      buffer_->position = {pos.x + g.planeBounds.left * fs, pos.y + g.planeBounds.top * fs};
      buffer_->texCoords = {g.atlasBounds.left, g.atlasBounds.top};
      buffer_->color = text.color();
      buffer_->outlineColor = text.outlineColor();
      buffer_->texHandle = *reinterpret_cast<const vec2u *>(&handle);
      buffer_->data = {outlineWidth, outlineEdge};
      buffer_++;

      pos.x += g.advance * fs;

      verticesCount_ += 4;
      indicesCount_ += 6;
    }
  }

  void TextRenderer::begin() {
    cameraData_.view = camera_->view();
    cameraData_.proj = camera_->projection();

    indicesCount_ = 0;
    verticesCount_ = 0;
    buffer_ = &bufferData_[0];
  }

  void TextRenderer::end() {
    if (!verticesCount_) {
      return;
    }

    DisplayList commands;
    commands.setupViewCommand()
        .set_viewport(camera_->viewport())
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

    //render
    commands.fillBufferCommand()
        .set_buffer(*vertexBuffer_)
        .set_data(&bufferData_[0])
        .set_size(verticesCount_ * sizeof(VertexData));

    commands.setupPipelineCommand()
        .set_pipeline(*pipeline_)
        .set_buffer(0, *vertexBuffer_)
        .set_uniformBuffer(0, *uniformBuffer_);

    commands.renderCommand()
        .set_indexBuffer(*indexBuffer_)
        .set_count(indicesCount_)
        .set_type(IndexFormat::Uint32);
    Engine::ref().executeOnGpu(std::move(commands));
  }

}
