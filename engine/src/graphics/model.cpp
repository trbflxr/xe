//
// Created by trbflxr on 3/29/2020.
//

#include "xepch.hpp"
#include <xe/graphics/model.hpp>

#include <xe/core/engine.hpp>
#include <utils/model_loader.hpp>

namespace xe {

  Model::Model(std::string_view file) {
    ModelLoader::load(file, this);

    //create texture
    gpu::Texture::Info info;
    info.minFilter = TextureMinFilter::Linear;
    info.magFilter = TextureMagFilter::Linear;
//    info.bindless = true;

    texture_ = std::make_shared<Texture>();
    texture_->setInfo(info);
    texture_->loadFromFile("assets/models/Suzanne/Suzanne_BaseColor.png", false);
    texture_->setup();

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
    pipeline_ = Engine::ref().gpu().createPipeline(matInfo);

    for (auto &&m : meshes_) {
      m.createBuffers();
    }
  }

  void Model::destroy() {
    texture_->destroy();
    Engine::ref().gpu().destroyResource(*pipeline_);

    for (auto &&m : meshes_) {
      m.destroy();
    }
  }

  void Model::render(DisplayList &dl, const Camera &camera) {
    for (const auto &m : meshes_) {
      dl.setupPipelineCommand()
          .set_pipeline(*pipeline_)
          .set_buffer(0, *m.gpu_.vertices.buffer)
          .set_uniformBuffer(0, camera.uniformBuffer())
          .set_texture(0, texture_->raw());
      dl.renderCommand()
          .set_indexBuffer(*m.gpu_.indices.buffer)
          .set_count(static_cast<uint32_t>(m.gpu_.indices.data.size()))
          .set_type(IndexFormat::Uint32);
    }
  }

}
