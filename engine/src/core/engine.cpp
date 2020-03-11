//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/engine.hpp>

#include "global_config.hpp"
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

  int32_t Engine::run() {
    XE_TRACE_INIT("xetrace.json");
    XE_TRACE_META_THREAD_NAME("Update thread");

    XE_TRACE_BEGIN("XE", "Application init");
    XE_CORE_INFO("[Engine] Starting application");
    if (!init()) {
      XE_CORE_CRITICAL("[Engine] Engine initialization failed");
      return 1;
    }
    app_->onInit();
    XE_TRACE_END("XE", "Application init");

    XE_TRACE_BEGIN("XE", "Application start");
    start();
    XE_TRACE_END("XE", "Application start");

    XE_CORE_INFO("[Engine] First application loop reached");

    Timestep lastTime = Engine::ref().window().uptime();
    Timestep accumulator;
    Timestep uptime;
    uint32_t updates = 0;
    uint32_t frames = 0;

    while (!isRunning()) {
      XE_TRACE_BEGIN("XE", "Frame");
      XE_CORE_TRACE("[Engine] New render frame (FPS: {}) (UPS: {})", framerate_.fps, updates);

      Timestep now = Engine::ref().window().uptime();
      Timestep deltaTime = now - lastTime;
      lastTime = now;

      if ((Engine::ref().window().uptime() - uptime) > 1.0f) {
        uptime += 1.0f;
        framerate_.fps = frames;
        frames = 0;
      }

      updates = 0;
      accumulator += deltaTime;

      XE_TRACE_BEGIN("XE", "Loop update");
      preUpdate();

      while (accumulator > framerate_.timeStep) {
        if (updates >= framerate_.maxSteps) {
          XE_CORE_WARN("[Engine] Exceeded max steps per frame (Consider lowering timeStep)");
          break;
        }

        XE_TRACE_BEGIN("XE", "Application update");
        update(delta());
        XE_TRACE_END("XE", "Application update");

        accumulator -= framerate_.timeStep;
        ++updates;
      }

      postUpdate();
      XE_TRACE_END("XE", "Loop update");

      XE_TRACE_BEGIN("XE", "Application process events");
      processEvents();
      XE_TRACE_END("XE", "Application process events");

      XE_TRACE_BEGIN("XE", "Render update");
      renderPreUpdate();
      renderUpdate();
      renderPostUpdate();
      XE_TRACE_END("XE", "Render update");

      ++frames;
      XE_TRACE_END("XE", "Frame");
    }
    XE_CORE_INFO("[Engine] Stopping application");
    stop();

    XE_TRACE_SHUTDOWN();

    XE_CORE_INFO("[Engine] Application exit code 0");
    return 0;
  }

  void Engine::requestClose() {
    gpu_->window_->forceExit();
  }

  bool Engine::isRunning() const {
    return gpu_->isExisting();
  }

  void Engine::submitDrawList(DisplayList &&dl) {
    gpu_->submitCommands(std::move(dl));
  }

  void Engine::setParams(const Params &params) {
    auto &config = GlobalConfig::ref();

    XE_ASSERT(!config.initialized_, "[Engine] Params are already set");

    config.params_ = params;
    config.initialized_ = true;

    gpu_->params_ = config.params_.gpu;
    gpu_->window_->params_ = config.params_.window;
  }

  Params Engine::getParams() const {
    return GlobalConfig::ref().getParams();
  }

  void Engine::setUiFunction(const std::function<void(void *)> &function, void *data) {
    gpu_->window_->ui_ = function;
    gpu_->window_->uiData_ = data;
  }

  bool Engine::init() {
    XE_TRACE_BEGIN("XE", "Engine systems init");
    XE_CORE_INFO("[Engine] Initializing engine systems");

    transform_ = std::make_shared<System::Transform>();

    gpu_->init();
    assetManager_->init();

    XE_TRACE_END("XE", "Engine systems init");
    return true;
  }

  void Engine::start() {
    app_->onStart();

    if (!scene_) return;

    startSystems();
  }

  void Engine::preUpdate() {
    app_->onPreUpdate();

    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems pre update");

    transform_->preUpdate();

    XE_TRACE_END("XE", "Engine systems pre update");
  }

  void Engine::update(Timestep /*ts*/) {
    app_->onUpdate();

    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems update");

    XE_TRACE_END("XE", "Engine systems update");
  }

  void Engine::postUpdate() {
    app_->onPostUpdate();

    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems post update");

    XE_TRACE_END("XE", "Engine systems post update");
  }

  void Engine::renderPreUpdate() {
    app_->onRreRender();

    gpu_->prepareRender();

    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems pre render");

    XE_TRACE_END("XE", "Engine systems pre render");
  }

  void Engine::renderUpdate() {
    app_->onRender();

    if (!scene_) return;

    XE_TRACE_BEGIN("XE", "Engine systems render");

    XE_TRACE_END("XE", "Engine systems render");
  }

  void Engine::renderPostUpdate() {
    app_->onPostRender();

    if (scene_) {
      XE_TRACE_BEGIN("XE", "Engine systems post render");

      XE_TRACE_END("XE", "Engine systems post render");
    }

    gpu_->execute();
  }

  void Engine::stop() {
    app_->onStop();

    gpu_->stop();
    stopSystems();
  }

  void Engine::processEvents() {
    Event e{ };
    while (Engine::ref().window().pollEvent(e)) {
      app_->onEvent(e);

      if (e.type == Event::Closed) {
        requestClose();
        break;
      }
    }
  }

  void Engine::startSystems() {
    XE_TRACE_BEGIN("XE", "Engine systems start");
    XE_CORE_INFO("[Engine] Engine start");

    XE_TRACE_END("XE", "Engine systems start");
  }

  void Engine::stopSystems() {
    XE_TRACE_BEGIN("XE", "Engine systems stop");
    XE_CORE_INFO("[Engine] Engine stop");

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
