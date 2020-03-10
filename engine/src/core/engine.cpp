//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/engine.hpp>

#include <xe/core/vfs.hpp>

namespace xe {

  Engine *Engine::instance_ = nullptr;

  Engine::Engine(int32_t argc, char **argv) {
    setName("Engine");

    XE_ASSERT(!instance_, "[Engine] Engine is already created");
    instance_ = this;

    for (int32_t i = 0; i < argc; ++i) {
      args_.emplace_back(argv[i]);
    }

    XE_TRACE_BEGIN("XE", "Creating modules");
    vfs_.reset(new VFS());
    gpu_.reset(new GPU());
    assetManager_.reset(new AssetManager());
    XE_TRACE_END("XE", "Creating modules");
  }

  Engine::~Engine() {

  }

  bool Engine::isExisting() const {
    return gpu_->isExisting();
  }

  void Engine::submitDrawList(DisplayList &&dl) {
    gpu_->submitCommands(std::move(dl));
  }

  void Engine::setParams(const Params &params) {
    params_ = params;
    gpu_->setParams(params_.gpu);
    gpu_->window_->setParams(params_.window);
  }

  void Engine::setUiFunction(const std::function<void(void *)> &function, void *data) {
    gpu_->window_->ui_ = function;
    gpu_->window_->uiData_ = data;
  }

  bool Engine::init() {
    XE_TRACE_BEGIN("XE", "Engine systems init");
    XE_CORE_INFO("[ENGINE] Initializing engine systems");

    transform_ = std::make_shared<System::Transform>();

    gpu_->init();
    assetManager_->init();

    XE_TRACE_END("XE", "Engine systems init");
    return true;
  }

  void Engine::start() {
    if (!scene_) return;

    startSystems();
  }

  void Engine::preUpdate() {
    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems pre update");

    transform_->preUpdate();

    XE_TRACE_END("XE", "Engine systems pre update");
  }

  void Engine::update(Timestep /*ts*/) {
    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems update");

    XE_TRACE_END("XE", "Engine systems update");
  }

  void Engine::postUpdate() {
    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems post update");

    XE_TRACE_END("XE", "Engine systems post update");
  }

  void Engine::renderPreUpdate() {
    gpu_->prepareRender();

    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems pre render");

    XE_TRACE_END("XE", "Engine systems pre render");
  }

  void Engine::renderUpdate() {
    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems render");

    XE_TRACE_END("XE", "Engine systems render");
  }

  void Engine::renderPostUpdate() {
    if (scene_) {
      XE_TRACE_BEGIN("XE", "Engine systems post render");

      XE_TRACE_END("XE", "Engine systems post render");
    }

    gpu_->execute();
  }

  void Engine::stop() {
    gpu_->stop();
    stopSystems();
  }

  void Engine::startSystems() {
    XE_TRACE_BEGIN("XE", "Engine systems start");
    XE_CORE_INFO("[ENGINE] Engine start");

    XE_TRACE_END("XE", "Engine systems start");
  }

  void Engine::stopSystems() {
    XE_TRACE_BEGIN("XE", "Engine systems stop");
    XE_CORE_INFO("[ENGINE] Engine stop");

    XE_TRACE_END("XE", "Engine systems stop");
  }

  void Engine::loadScene(const std::shared_ptr<Scene> &scene) {
    XE_TRACE_BEGIN("XE", "Load scene");

    if (!scene) {
      XE_CORE_ERROR("[Engine] Could not load scene (nullptr)");
      return;
    }

    stopSystems();

    scene_ = scene;

    transform_->onSceneChanged();

    startSystems();

    XE_TRACE_END("XE", "Load scene");
  }

  bool Engine::isKeyPressed(Keyboard::Key key) {
    return ref().window().isKeyPressed(key);
  }

  bool Engine::isMouseButtonPressed(Mouse::Button button) {
    return ref().window().isMouseButtonPressed(button);
  }

  vec2 Engine::getMousePosition() {
    return ref().window().getMousePosition();
  }

  void Engine::setMousePosition(const vec2 &position) {
    ref().window().setMousePosition(position);
  }

}
