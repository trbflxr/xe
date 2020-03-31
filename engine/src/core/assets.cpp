//
// Created by FLXR on 9/13/2019.
//

#include "xepch.hpp"
#include <xe/core/assets.hpp>

#include <xe/utils/debug_trace.hpp>
#include <xe/core/vfs.hpp>

#include "embedded/embedded.hpp"

namespace xe {

  void AssetManager::init() {
    initDefaultShaders();
  }

  void AssetManager::initDefaultShaders() {
    XE_TRACE_BEGIN("XE", "Default shaders init");

    auto final = std::make_shared<gpu::Pipeline::Info::Shader>();
    final->vert = gpu::Pipeline::makeShader({Embedded::shaderSource("final.vert")});
    final->frag = gpu::Pipeline::makeShader({Embedded::shaderSource("final.frag")});
    addShader("final", final);

    auto renderer2d = std::make_shared<gpu::Pipeline::Info::Shader>();
    renderer2d->vert = gpu::Pipeline::makeShader({Embedded::shaderSource("renderer2d.vert")});
    renderer2d->frag = gpu::Pipeline::makeShader({Embedded::shaderSource("renderer2d.frag")});
    addShader("renderer2d", renderer2d);

    auto textRenderer = std::make_shared<gpu::Pipeline::Info::Shader>();
    textRenderer->vert = gpu::Pipeline::makeShader({Embedded::shaderSource("text_renderer.vert")});
    textRenderer->frag = gpu::Pipeline::makeShader({Embedded::shaderSource("text_renderer.frag")});
    addShader("text_renderer", textRenderer);

    auto model = std::make_shared<gpu::Pipeline::Info::Shader>();
    model->vert = gpu::Pipeline::makeShader({Embedded::shaderSource("ubo_camera3d.glsl"), Embedded::shaderSource("model.vert")});
    model->frag = gpu::Pipeline::makeShader({Embedded::shaderSource("model.frag")});
    addShader("model", model);

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
