//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/engine.hpp>

namespace xe {

  Engine::Engine() {
    setName("Engine");

    gpu_ = make_ref<GPU>();
  }

  Engine::~Engine() {

  }

  Engine &Engine::ref() {
    static scoped_ptr<Engine> e;
    if (!e) {
      e.reset(new Engine());
    }
    return *e;
  }

  void Engine::setParams(const Params &params) {
    params_ = params;
    gpu_->setParams(params_.gpu);
    gpu_->window_->setParams(params_.window);
  }

  bool Engine::init() {
    XE_TRACE_BEGIN("XE", "Engine systems init");
    XE_CORE_INFO("[ENGINE] Initializing engine systems");

    gpu_->init();

    XE_TRACE_END("XE", "Engine systems init");
    return true;
  }

  void Engine::start() {
    startSystems();
  }

  void Engine::preUpdate() {

  }

  void Engine::update(Timestep ts) {

  }

  void Engine::postUpdate() {

  }

  void Engine::renderPreUpdate() {
    gpu_->prepareRender();
  }

  void Engine::renderUpdate() {

  }

  void Engine::renderPostUpdate() {
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

  bool Engine::isExisting() const {
    return gpu_->isExisting();
  }

  void Engine::submitDrawList(DisplayList &&dl) {
    gpu_->submitCommands(std::move(dl));
  }

  void Engine::setUiFunction(const std::function<void(void *)> &function, void *data) {
    gpu_->window_->ui_ = function;
    gpu_->window_->uiData_ = data;
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
