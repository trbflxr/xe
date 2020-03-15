//
// Created by trbflxr on 3/14/2020.
//

#include "xepch.hpp"
#include <xe/graphics/composer.hpp>

#include <xe/core/engine.hpp>

namespace xe {

  static float composerVertexData[] = {
      -1.0, -1.0,
      1.0, -1.0,
      1.0, 1.0,
      -1.0, 1.0
  };

  static uint16_t composerIndexData[] = {0, 2, 1, 0, 3, 2};


  Composer::Composer() {
    setName("Composer");

  }

  void Composer::init(const vec2u &size, TexelsFormat format) {
    size_ = size;
    format_ = format;

    gpu::Texture::Info colorAttachment0 = {size_.x, size_.y};
    colorAttachment0.format = format_;

    gpu::Texture::Info depthAttachment = {size_.x, size_.y};
    depthAttachment.format = TexelsFormat::Depth16;

    gpu::Framebuffer::Info fbInfo;
    fbInfo.colorAttachmentInfo[0] = colorAttachment0;
    fbInfo.depthStencilAttachmentInfo = depthAttachment;
    fbInfo.colorAttachmentsSize = 1;

    framebuffer_ = Engine::ref().gpu().createFramebuffer(fbInfo);

    //dummy quad
    quad_.vertexBuffer = Engine::ref().gpu().createBuffer(
        {BufferType::Vertex, Usage::Static, sizeof(composerVertexData)});
    quad_.indexBuffer = Engine::ref().gpu().createBuffer(
        {BufferType::Index, Usage::Static, sizeof(composerIndexData)});

    gpu::Pipeline::Info matInfo;
    matInfo.shader = *Engine::ref().assetManager().getShader("final");

    matInfo.attribs[0] = {"a_position", VertexFormat::Float2};

    matInfo.textures[0] = TextureType::T2D;
    matInfo.cull = Cull::Disabled;
    quad_.material = Engine::ref().gpu().createPipeline(matInfo);

    DisplayList frame;
    frame.fillBufferCommand()
        .set_buffer(quad_.vertexBuffer)
        .set_data(composerVertexData)
        .set_size(sizeof(composerVertexData));
    frame.fillBufferCommand()
        .set_buffer(quad_.indexBuffer)
        .set_data(composerIndexData)
        .set_size(sizeof(composerIndexData));
    Engine::ref().submitDrawList(std::move(frame));
  }

  void Composer::present() const {
    DisplayList frame;

    frame.setupViewCommand()
        .set_viewport({0, 0, size_.x, size_.y});
    frame.clearCommand()
        .set_color(Color::Fuchsia)
        .set_clearColor(true)
        .set_clearDepth(true);
    frame.setupPipelineCommand()
        .set_pipeline(quad_.material)
        .set_buffer(0, quad_.vertexBuffer)
        .set_texture(0, targetTexture());
    frame.renderCommand()
        .set_indexBuffer(quad_.indexBuffer)
        .set_count(sizeof(composerIndexData) / sizeof(uint16_t))
        .set_type(IndexFormat::Uint16);

    Engine::ref().submitDrawList(std::move(frame));
  }

}
