//
// Created by FLXR on 9/13/2019.
//

#include "xepch.hpp"
#include <xe/core/assets.hpp>
#include <xe/utils/debug_trace.hpp>
#include <xe/core/vfs.hpp>
#include "graphics/shaders.hpp"


namespace xe {

  static std::string makeShader(const std::initializer_list<const char *> &sources) {
    //todo: get version from somewhere
    static const char *version = "#version 460 core\n";
    std::string shader = version;

    for (const auto &s : sources) {
      shader += s;
    }

    return shader;
  }

  AssetManager::AssetManager() {

  }

  void AssetManager::init() {
    initDefaultShaders();
  }

  void AssetManager::initDefaultShaders() {
    XE_TRACE_BEGIN("XE", "Default shaders init");

    auto test = std::make_shared<gpu::Pipeline::Info::Shader>();
    test->vert = makeShader({ubo_test_state_glsl, test_vert});
    test->frag = makeShader({test_frag});
    addShader("test", test);

    auto fb_test = std::make_shared<gpu::Pipeline::Info::Shader>();
    fb_test->vert = makeShader({ubo_test_state_glsl, fb_test_vert});
    fb_test->frag = makeShader({fb_test_frag});
    addShader("fb_test", fb_test);

    auto final = std::make_shared<gpu::Pipeline::Info::Shader>();
    final->vert = makeShader({final_vert});
    final->frag = makeShader({final_frag});
    addShader("final", final);

    auto renderer2d = std::make_shared<gpu::Pipeline::Info::Shader>();
    renderer2d->vert = makeShader({renderer2d_vert});
    renderer2d->frag = makeShader({renderer2d_frag});
    addShader("renderer2d", renderer2d);

    auto textRenderer = std::make_shared<gpu::Pipeline::Info::Shader>();
    textRenderer->vert = makeShader({text_renderer_vert});
    textRenderer->frag = makeShader({text_renderer_frag});
    addShader("text_renderer", textRenderer);

    XE_TRACE_END("XE", "Default shaders init");
  }

  void AssetManager::addShader(const std::string &name, const std::shared_ptr<gpu::Pipeline::Info::Shader> &shader) {
    auto &&it = shaders_.find(name);
    if (it == shaders_.end()) {
      shaders_.emplace(name, shader);
      return;
    }
    XE_CORE_ERROR("[AssetManager] Shader '{}' already exists", name);
  }

  const std::shared_ptr<gpu::Pipeline::Info::Shader> &AssetManager::getShader(const std::string &name) {
    static const std::shared_ptr<gpu::Pipeline::Info::Shader> empty;

    auto &&it = shaders_.find(name);
    if (it != shaders_.end()) {
      return it->second;
    }
    XE_CORE_ERROR("[AssetManager] Could not find shader '{}'", name);
    return empty;
  }

}
