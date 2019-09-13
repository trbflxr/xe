//
// Created by FLXR on 9/13/2019.
//

#include "xepch.hpp"
#include <xe/core/assets.hpp>
#include <xe/utils/debug_trace.hpp>
#include <xe/core/vfs.hpp>
#include "graphics/shaders.hpp"


namespace xe {

  AssetManager::AssetManager() {

  }

  void AssetManager::init() {
    initDefaultShaders();
  }

  void AssetManager::initDefaultShaders() {
    auto test = make_ref<gpu::Pipeline::Info::Shader>();
    test->vert = test_vert;
    test->frag = test_frag;

    addShader("test", test);

    auto fb_test = make_ref<gpu::Pipeline::Info::Shader>();
    fb_test->vert = fb_test_vert;
    fb_test->frag = fb_test_frag;

    addShader("fb_test", fb_test);
  }

  void AssetManager::addShader(const string &name, const ref_ptr<gpu::Pipeline::Info::Shader> &shader) {
    auto &&it = shaders_.find(name);
    if (it == shaders_.end()) {
      shaders_.emplace(name, shader);
      return;
    }
    XE_CORE_ERROR("[AssetManager] Shader '{}' already exists", name);
  }

  const ref_ptr<gpu::Pipeline::Info::Shader> &AssetManager::getShader(const string &name) {
    static const ref_ptr<gpu::Pipeline::Info::Shader> empty;

    auto &&it = shaders_.find(name);
    if (it != shaders_.end()) {
      return it->second;
    }
    XE_CORE_ERROR("[AssetManager] Could not find shader '{}'", name);
    return empty;
  }

}
