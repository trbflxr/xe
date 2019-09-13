//
// Created by FLXR on 9/13/2019.
//

#ifndef XE_ASSETS_HPP
#define XE_ASSETS_HPP

#include <xe/common.hpp>
#include <xe/unordered_map.hpp>
#include <xe/core/object.hpp>
#include <xe/utils/noncopyable.hpp>
#include <xe/graphics/gpu_resources.hpp>

namespace xe {

  class XE_API AssetManager : public Object, NonCopyable {
  XE_OBJECT(AssetManager, Object);
    friend class Engine;
  public:
    void init();

    void addShader(const string &name, const ref_ptr<gpu::Pipeline::Info::Shader> &shader);
    const ref_ptr<gpu::Pipeline::Info::Shader> &getShader(const string &name);

  private:
    AssetManager();

    void initDefaultShaders();

  private:
    unordered_map<string, ref_ptr<gpu::Pipeline::Info::Shader>> shaders_;
  };

}

#endif //XE_ASSETS_HPP
