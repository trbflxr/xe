//
// Created by trbflxr on 4/3/2020.
//

#include "xepch.hpp"
#include <xe/graphics/mesh.hpp>

#include <xe/core/engine.hpp>

namespace xe {

  Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<uint32_t> &&indices, uint32_t matIndex, Usage usage) :
      matIndex_(matIndex) {
    gpu_.vertices.data = std::move(vertices);
    gpu_.indices.data = std::move(indices);
    gpu_.usage = usage;
  }

  void Mesh::createBuffers() {
    const uint32_t vbSize = static_cast<uint32_t>(gpu_.vertices.data.size()) * sizeof(Vertex);
    const uint32_t ibSize = static_cast<uint32_t>(gpu_.indices.data.size()) * sizeof(uint32_t);

    gpu_.vertices.buffer = Engine::ref().gpu().createBuffer({BufferType::Vertex, gpu_.usage, vbSize});
    gpu_.indices.buffer = Engine::ref().gpu().createBuffer({BufferType::Index, gpu_.usage, ibSize});

    DisplayList commands;
    commands.fillBufferCommand()
        .set_buffer(*gpu_.vertices.buffer)
        .set_data(gpu_.vertices.data.data())
        .set_size(vbSize);
    commands.fillBufferCommand()
        .set_buffer(*gpu_.indices.buffer)
        .set_data(gpu_.indices.data.data())
        .set_size(ibSize);
    Engine::ref().executeOnGpu(std::move(commands));
  }

  void Mesh::destroy() {
    if (gpu_.vertices.buffer) {
      Engine::ref().gpu().destroyResource(*gpu_.vertices.buffer);
    }
    if (gpu_.indices.buffer) {
      Engine::ref().gpu().destroyResource(*gpu_.indices.buffer);
    }
  }

}
