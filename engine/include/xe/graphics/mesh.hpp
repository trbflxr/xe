//
// Created by trbflxr on 4/3/2020.
//

#ifndef XE_MESH_HPP
#define XE_MESH_HPP

#include <xe/math/vec2.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/gpu_resource.hpp>
#include <xe/graphics/gpu_resources.hpp>

namespace xe {

  struct XE_API Vertex {
    //structs because xe::vec3 is actually 4-float (simd)
    struct {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
    } position;
    struct {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
    } normal;
    struct {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
    } tangent;
    struct {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
    } biTangent;
    vec2 texCoords;
  };

  class XE_API Mesh : GPUResource {
    friend class Model;
  public:
    Mesh() = default;
    Mesh(std::vector<Vertex> &&vertices, std::vector<uint32_t> &&indices, uint32_t matIndex, Usage usage);

    void destroy() override;

  private:
    void createBuffers();

  private:
    struct {
      struct {
        std::vector<Vertex> data;
        std::shared_ptr<gpu::Buffer> buffer;
      } vertices;
      struct {
        std::vector<uint32_t> data;
        std::shared_ptr<gpu::Buffer> buffer;
      } indices;
      Usage usage;
    } gpu_;
    int32_t matIndex_ = -1;
  };

}

#endif //XE_MESH_HPP
