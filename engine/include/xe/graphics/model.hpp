//
// Created by trbflxr on 3/29/2020.
//

#ifndef XE_MODEL_HPP
#define XE_MODEL_HPP

#include <xe/math/vec2.hpp>
#include <xe/core/object.hpp>

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

  struct XE_API Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    Mesh() = default;
    Mesh(std::vector<Vertex> &&vertices, std::vector<uint32_t> &&indices) :
        vertices(std::move(vertices)),
        indices(std::move(indices)) { }
  };

  class XE_API Model : public Object {
  XE_OBJECT(Model, Object);
    friend class ModelLoader;
  public:
    explicit Model(std::string_view file);

//  private:
    //todo: materials
    std::vector<Mesh> meshes_;
  };

}

#endif //XE_MODEL_HPP
