//
// Created by trbflxr on 4/3/2020.
//

#include "xepch.hpp"
#include <xe/graphics/material.hpp>

#include <xe/core/engine.hpp>

namespace xe {

  void Material::setup() {
    //create texture
    gpu::Texture::Info info;
    info.minFilter = TextureMinFilter::Linear;
    info.magFilter = TextureMagFilter::Linear;
    info.bindless = true;

    gpu_.diffuse = std::make_shared<Texture>();
    gpu_.diffuse->setInfo(info);
    gpu_.diffuse->loadFromFile("assets/models/Suzanne/Suzanne_BaseColor.png", false);
    gpu_.diffuse->setup();

    gpu_.normals = std::make_shared<Texture>();
    gpu_.normals->setInfo(info);
    gpu_.normals->loadFromFile("assets/models/Suzanne/Suzanne_MetallicRoughness.png", false);
    gpu_.normals->setup();

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
    gpu_.pipeline = Engine::ref().gpu().createPipeline(matInfo);

    gpu_.buffer = Engine::ref().gpu().createBuffer(
        {BufferType::Uniform, Usage::Dynamic, sizeof(MaterialData), "PBRMaterial", cPBRUboBinding});

    DisplayList commands;
    commands.fillBufferCommand()
        .set_buffer(*gpu_.buffer)
        .set_data(&gpu_.data)
        .set_size(sizeof(MaterialData));
    Engine::ref().executeOnGpu(std::move(commands));
  }

  void Material::destroy() {
    if (gpu_.diffuse) {
      gpu_.diffuse->destroy();
    }
    if (gpu_.normals) {
      gpu_.normals->destroy();
    }
    if (gpu_.buffer) {
      Engine::ref().gpu().destroyResource(*gpu_.buffer);
    }
    if (gpu_.pipeline) {
      Engine::ref().gpu().destroyResource(*gpu_.pipeline);
    }
  }

  void Material::update() {
    if (updated_) {
      return;
    }
    //textures are bindless so it works fine
    bool upd = true;
    if (gpu_.diffuse) {
      const uint64_t handle = gpu_.diffuse->handle();
      gpu_.data.diffuse = *reinterpret_cast<const vec2u *>(&handle);
      upd = handle;
    }
    if (gpu_.normals && upd) {
      const uint64_t handle = gpu_.normals->handle();
      gpu_.data.normals = *reinterpret_cast<const vec2u *>(&handle);
      upd = handle;
    }

    if (upd) {
      DisplayList commands;
      commands.fillBufferCommand()
          .set_buffer(*gpu_.buffer)
          .set_data(&gpu_.data)
          .set_size(sizeof(MaterialData));
      Engine::ref().executeOnGpu(std::move(commands));

      updated_ = upd;
    }
  }

}
