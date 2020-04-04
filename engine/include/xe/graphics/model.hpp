//
// Created by trbflxr on 3/29/2020.
//

#ifndef XE_MODEL_HPP
#define XE_MODEL_HPP

#include <xe/graphics/mesh.hpp>
#include <xe/graphics/camera.hpp>
#include <xe/graphics/material.hpp>

namespace xe {

  class XE_API Model : public Object, GPUResource {
  XE_OBJECT(Model, Object);
    friend class ModelLoader;
  public:
    explicit Model(std::string_view file);

    void destroy() override;

    void render(DisplayList &dl, const Camera &camera);

  private:
    std::vector<Mesh> meshes_;
    std::vector<std::shared_ptr<Material>> materials_;
    std::shared_ptr<gpu::Pipeline> pipeline_;
  };

}

#endif //XE_MODEL_HPP
