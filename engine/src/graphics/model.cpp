//
// Created by trbflxr on 3/29/2020.
//

#include "xepch.hpp"
#include <xe/graphics/model.hpp>

#include <xe/core/engine.hpp>
#include <utils/model_loader.hpp>
#include <xe/graphics/material.hpp>

namespace xe {

  Model::Model(std::string_view file) {
    ModelLoader::load(file, this);

    mat_ = std::make_shared<Material>();
    mat_->setup();

    for (auto &&m : meshes_) {
      m.createBuffers();
    }
  }

  void Model::destroy() {
    mat_->destroy();
    for (auto &&m : meshes_) {
      m.destroy();
    }
  }

  void Model::render(DisplayList &dl, const Camera &camera) {
    mat_->update();

    for (const auto &m : meshes_) {
      dl.setupPipelineCommand()
          .set_pipeline(*mat_->gpu_.pipeline)
          .set_buffer(0, *m.gpu_.vertices.buffer)
          .set_uniformBuffer(0, camera.uniformBuffer())
          .set_uniformBuffer(1, *mat_->gpu_.buffer);
      dl.renderCommand()
          .set_indexBuffer(*m.gpu_.indices.buffer)
          .set_count(static_cast<uint32_t>(m.gpu_.indices.data.size()))
          .set_type(IndexFormat::Uint32);
    }
  }

}
