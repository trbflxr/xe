//
// Created by FLXR on 9/4/2019.
//

#include "xepch.hpp"
#include "events/event.hpp"
#include <xe/core/application.hpp>
#include <xe/core/engine.hpp>

namespace xe {

  Application::Application() :
      exit_(false) {
    setName("Application");

    framerate_.timeStep = 1.0f / 60.0f;
    framerate_.maxSteps = 10;
  }

  Application::~Application() {

  }

  void Application::initInternal() {
    if (!Engine::ref().init()) {
      exit_ = true;
      XE_CORE_CRITICAL("[Application] Engine initialization failed");
      return;
    }
    init();
  }

  void Application::startInternal() {
    start();
    Engine::ref().start();
  }

  void Application::preUpdateInternal() {
    preUpdate();
    Engine::ref().preUpdate();
  }

  void Application::updateInternal(Timestep ts) {
    update(ts);
    Engine::ref().update(ts);
  }

  void Application::postUpdateInternal() {
    postUpdate();
    Engine::ref().postUpdate();
  }

  void Application::renderPreUpdateInternal() {
    renderPreUpdate();
    Engine::ref().renderPreUpdate();
  }

  void Application::renderUpdateInternal() {
    renderUpdate();
    Engine::ref().renderUpdate();
  }

  void Application::renderPostUpdateInternal() {
    renderPostUpdate();
    Engine::ref().renderPostUpdate();
  }

  void Application::stopInternal() {
    stop();
    Engine::ref().stop();
  }

  void Application::processEvents() {
    Event e;
    while (Engine::ref().window().pollEvent(e)) {
      if (e.type == Event::MouseMoved) {
        XE_CORE_CRITICAL("ms({}, {})", e.mouseMove.x, e.mouseMove.y);
      }

      if (e.type == Event::Closed) {
        Engine::ref().window().forceExit();
        break;
      }
    }
  }

  bool Application::isExisting() const {
    return Engine::ref().isExisting();
  }

  int32 Application::run(int32 argc, char **argv) {
    XE_TRACE_INIT("xetrace.json");
    XE_TRACE_META_THREAD_NAME("Update thread");

    XE_TRACE_BEGIN("XE", "Application init");
    XE_CORE_INFO("[ENGINE] Starting application");
    initInternal();

    if (exit_) {
      return 0;
    }

    XE_TRACE_END("XE", "Application init");

    XE_TRACE_BEGIN("XE", "Application start");
    startInternal();
    XE_TRACE_END("XE", "Application start");

    XE_CORE_INFO("[ENGINE] First application loop reached");

    Timestep lastTime = Engine::ref().window().uptime();
    Timestep accumulator;
    Timestep uptime;
    uint updates = 0;
    uint frames = 0;

    while (!isExisting()) {
      XE_TRACE_BEGIN("XE", "Frame");
      XE_CORE_TRACE("[ENGINE] New render frame (FPS: {}) (UPS: {})", framerate_.fps, updates);

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
      preUpdateInternal();

      while (accumulator > framerate_.timeStep) {
        if (updates >= framerate_.maxSteps) {
          XE_CORE_WARN("[ENGINE] Exceeded max steps per frame (Consider lowering timeStep)");
          break;
        }

        XE_TRACE_BEGIN("XE", "Application update");
        updateInternal(delta());
        XE_TRACE_END("XE", "Application update");

        accumulator -= framerate_.timeStep;
        ++updates;
      }

      postUpdateInternal();
      XE_TRACE_END("XE", "Loop update");

      XE_TRACE_BEGIN("XE", "Application process events");
      processEvents();
      XE_TRACE_END("XE", "Application process events");

      XE_TRACE_BEGIN("XE", "Render update");
      renderPreUpdateInternal();
      renderUpdateInternal();
      renderPostUpdateInternal();

      XE_TRACE_END("XE", "Render update");

      ++frames;
      XE_TRACE_END("XE", "Frame");
    }
    XE_CORE_INFO("[ENGINE] Stopping application");
    stopInternal();

    XE_TRACE_SHUTDOWN();

    XE_CORE_INFO("[ENGINE] Application exit code 0");
    return 0;
  }

}
