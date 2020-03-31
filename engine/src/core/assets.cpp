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
    initDefaultShaderSource();
    initDefaultShaders();
  }

  void AssetManager::initDefaultShaderSource() {
    XE_TRACE_BEGIN("XE", "Default shaders source init");

    addShaderSource("ubo_camera3d.glsl", Embedded::shaderSource("ubo_camera3d.glsl"));

    addShaderSource("final.vert", Embedded::shaderSource("final.vert"));
    addShaderSource("final.frag", Embedded::shaderSource("final.frag"));

    addShaderSource("renderer2d.vert", Embedded::shaderSource("renderer2d.vert"));
    addShaderSource("renderer2d.frag", Embedded::shaderSource("renderer2d.frag"));

    addShaderSource("text_renderer.vert", Embedded::shaderSource("text_renderer.vert"));
    addShaderSource("text_renderer.frag", Embedded::shaderSource("text_renderer.frag"));

    addShaderSource("model.vert", Embedded::shaderSource("model.vert"));
    addShaderSource("model.frag", Embedded::shaderSource("model.frag"));

    XE_TRACE_END("XE", "Default shader sources init");
  }

  void AssetManager::initDefaultShaders() {
    XE_TRACE_BEGIN("XE", "Default shaders init");

    auto final = std::make_shared<gpu::Pipeline::Info::Shader>();
    final->vert = gpu::Pipeline::makeShader({getShaderSource("final.vert")});
    final->frag = gpu::Pipeline::makeShader({getShaderSource("final.frag")});
    addShader("final", final);

    auto renderer2d = std::make_shared<gpu::Pipeline::Info::Shader>();
    renderer2d->vert = gpu::Pipeline::makeShader({getShaderSource("renderer2d.vert")});
    renderer2d->frag = gpu::Pipeline::makeShader({getShaderSource("renderer2d.frag")});
    addShader("renderer2d", renderer2d);

    auto textRenderer = std::make_shared<gpu::Pipeline::Info::Shader>();
    textRenderer->vert = gpu::Pipeline::makeShader({getShaderSource("text_renderer.vert")});
    textRenderer->frag = gpu::Pipeline::makeShader({getShaderSource("text_renderer.frag")});
    addShader("text_renderer", textRenderer);

    auto model = std::make_shared<gpu::Pipeline::Info::Shader>();
    model->vert = gpu::Pipeline::makeShader({getShaderSource("ubo_camera3d.glsl"), getShaderSource("model.vert")});
    model->frag = gpu::Pipeline::makeShader({getShaderSource("model.frag")});
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

  void AssetManager::addShaderSource(const std::string &name, std::string &&source) {
    auto &&it = shaderSources_.find(name);
    if (it == shaderSources_.end()) {
      shaderSources_.emplace(name, std::move(source));
      return;
    }
    XE_CORE_ERROR("[AssetManager] Shader source '{}' already exists", name);
  }

  std::string_view AssetManager::getShaderSource(const std::string &name) {
    static const std::string empty;

    auto &&it = shaderSources_.find(name);
    if (it != shaderSources_.end()) {
      return it->second;
    }
    XE_CORE_ERROR("[AssetManager] Could not find shader source '{}'", name);
    return empty;
  }

}
