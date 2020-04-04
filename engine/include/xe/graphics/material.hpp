//
// Created by trbflxr on 4/3/2020.
//

#ifndef XE_MATERIAL_HPP
#define XE_MATERIAL_HPP

#include <xe/graphics/texture.hpp>

namespace xe {

  struct XE_API MaterialData {
    Color mask = Color::White;
    vec2u diffuse;
    vec2u normals;
  };

  class XE_API Material : GPUResource {
    friend class Model;
    friend class ModelLoader;
  public:
    Material() = default;

    void setup();

    void update();

    void destroy() override;

  private:
    struct {
      std::shared_ptr<Texture> diffuse;
      std::shared_ptr<Texture> normals;
      MaterialData data;
    } gpu_;
    bool updated_ = false;
    int32_t id_ = -1;
  };

}

#endif //XE_MATERIAL_HPP
