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

    for (auto &&m : meshes_) {
      m.createBuffers();
    }

    for (auto &&m : materials_) {
      m->setup();
    }

    //setup pipeline
    gpu::Pipeline::Info matInfo;
    matInfo.shader = *Engine::ref().assetManager().getShader("model");

    matInfo.attribs[0] = {"a_position", VertexFormat::Float3};
    matInfo.attribs[1] = {"a_normal", VertexFormat::Float3};
    matInfo.attribs[2] = {"a_tangent", VertexFormat::Float3};
    matInfo.attribs[3] = {"a_biTangent", VertexFormat::Float3};
    matInfo.attribs[4] = {"a_texCoords", VertexFormat::Float2};

    matInfo.blend.enabled = true;
    matInfo.cull = Cull::Back;
    pipeline_ = Engine::ref().gpu().createPipeline(matInfo);
  }

  void Model::destroy() {
    for (auto &&m : meshes_) {
      m.destroy();
    }

    for (auto &&m : materials_) {
      m->destroy();
    }

    if (pipeline_) {
      Engine::ref().gpu().destroyResource(*pipeline_);
    }
  }

  void Model::render(DisplayList &dl, const Camera &camera) {
    for (const auto &m : meshes_) {
      if (m.matIndex_ == -1) {
        XE_CORE_WARN("[Model] Unable to render model. Materials are empty.");
        return;
      }

      auto &mat = materials_[static_cast<size_t>(m.matIndex_)];
      mat->update();

      dl.setupPipelineCommand()
          .set_pipeline(*pipeline_)
          .set_buffer(0, *m.gpu_.vertices.buffer)
          .set_uniformBuffer(0, camera.uniformBuffer())
          .set_uniform(0, {"u_diffuse", &mat->gpu_.data.diffuse, sizeof(vec2u)})
          .set_uniform(1, {"u_normals", &mat->gpu_.data.normals, sizeof(vec2u)})
          .set_uniform(2, {"u_mask", &mat->gpu_.data.mask, sizeof(Color)});

      dl.renderCommand()
          .set_indexBuffer(*m.gpu_.indices.buffer)
          .set_count(static_cast<uint32_t>(m.gpu_.indices.data.size()))
          .set_type(IndexFormat::Uint32);
    }
  }

}
