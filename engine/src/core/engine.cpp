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
    registry_ = std::make_unique<entt::registry>();
    vfs_.reset(new VFS());
    gpu_.reset(new GPU());
    assetManager_.reset(new AssetManager());

    composer_ = std::make_unique<Composer>();
    XE_TRACE_END("XE", "Creating modules");
  }

  Engine::~Engine() {
    gpu_->shouldStop_ = true;
    gpu_->stop();
  }

  void Engine::init(const Params &params) {
    auto &config = GlobalConfig::ref();

    XE_ASSERT(!config.initialized_, "[Engine] Params are already set");

    config.params_ = params;
    config.initialized_ = true;

    gpu_->params_ = config.params_.gpu;
    gpu_->window_->params_ = config.params_.window;
  }

  int32_t Engine::run() {
    running_ = true;

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

    while (running()) {
      XE_TRACE_BEGIN("XE", "Frame");

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
        framerate_.ups = updates;
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
    running_ = false;
    gpu_->window_->forceExit();
  }

  void Engine::setApp(std::shared_ptr<Application> &&app) {
    XE_TRACE_BEGIN("XE", "Set application");

    if (!app) {
      XE_CORE_ERROR("[Engine] Could not set application (nullptr)");
      return;
    }

    stopSystems();
    app_ = std::move(app);
    startSystems();

    XE_TRACE_END("XE", "Set application");
  }

  bool Engine::running() const {
    return running_;
  }

  void Engine::executeOnGpu(DisplayList &&dl) {
    gpu_->submitCommands(std::move(dl));
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

    gpu_->init();
    assetManager_->init();

    composer_->init({getParams().window.width, getParams().window.height}, TexelsFormat::Rgba16f);

    XE_TRACE_END("XE", "Engine systems init");
    return true;
  }

  void Engine::start() {
    if (!app_) return;
    app_->onStart();

    startSystems();
  }

  void Engine::preUpdate() {
    if (!app_) return;
    app_->onPreUpdate();

    XE_TRACE_BEGIN("XE", "Engine systems pre update");

    XE_TRACE_END("XE", "Engine systems pre update");
  }

  void Engine::update(Timestep /*ts*/) {
    if (!app_) return;
    app_->onUpdate();

    XE_TRACE_BEGIN("XE", "Engine systems update");

    XE_TRACE_END("XE", "Engine systems update");
  }

  void Engine::postUpdate() {
    if (!app_) return;
    app_->onPostUpdate();

    XE_TRACE_BEGIN("XE", "Engine systems post update");

    XE_TRACE_END("XE", "Engine systems post update");
  }

  void Engine::renderPreUpdate() {
    gpu_->prepareRender();

    if (!app_) return;
    app_->onPreRender();

    XE_TRACE_BEGIN("XE", "Engine systems pre render");

    XE_TRACE_END("XE", "Engine systems pre render");
  }

  void Engine::renderUpdate() {
    if (!app_) return;
    app_->onRender();

    XE_TRACE_BEGIN("XE", "Engine systems render");

    XE_TRACE_END("XE", "Engine systems render");
  }

  void Engine::renderPostUpdate() {
    if (app_) {
      app_->onPostRender();

      XE_TRACE_BEGIN("XE", "Engine systems post render");

      XE_TRACE_END("XE", "Engine systems post render");
    }

    gpu_->execute();
  }

  void Engine::stop() {
    if (app_) {
      app_->onStop();
    }
    stopSystems();
  }

  void Engine::processEvents() {
    Event e{ };
    while (!events_.empty()) {
      e = events_.front();
      events_.pop();

      if (app_) {
        app_->onEvent(e);
      }

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

  void Engine::pushEvents(const std::vector<Event> &events) {
    std::unique_lock<std::mutex> lock(eventsMutex_);
    for (const auto &e : events) {
      events_.push(e);
    }
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
