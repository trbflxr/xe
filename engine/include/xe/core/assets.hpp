//
// Created by FLXR on 9/13/2019.
//

#ifndef XE_ASSETS_HPP
#define XE_ASSETS_HPP

#include <unordered_map>
#include <xe/common.hpp>
#include <xe/core/object.hpp>
#include <xe/utils/noncopyable.hpp>
#include <xe/graphics/gpu_resources.hpp>

namespace xe {

  class XE_API AssetManager : public Object, NonCopyable {
  XE_OBJECT(AssetManager, Object);
    friend class Engine;
  public:
    void init();

    void addShader(const std::string &name, const std::shared_ptr<gpu::Pipeline::Info::Shader> &shader);
    const std::shared_ptr<gpu::Pipeline::Info::Shader> &getShader(const std::string &name);

  private:
    AssetManager();

    void initDefaultShaders();

  private:
    std::unordered_map<std::string, std::shared_ptr<gpu::Pipeline::Info::Shader>> shaders_;
  };

}

#endif //XE_ASSETS_HPP
