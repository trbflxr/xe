//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/graphics/window.hpp>
#include <xe/utils/debug_trace.hpp>

#ifdef XE_PLATFORM_GL
  #include "graphics/opengl/gl_window.hpp"
#else
  #error only gl currently supported
#endif

namespace xe {

  Window::Window() {
    setName("Window");
    data_ = new Data();
    data_->title = "XE Window";
  }

  Window::~Window() {
    delete data_;
  }

  void Window::init() {
    data_->width = params_.width;
    data_->height = params_.height;
    data_->title = params_.title;
    data_->swapInterval = params_.swapInterval;
    data_->icon.width = params_.iconWidth;
    data_->icon.height = params_.iconHeight;
    data_->icon.pixels = params_.iconPixels;

    WindowBackend::init(data_);
  }

  void Window::initContext() {
    WindowBackend::initContext(data_);
  }

  bool Window::isExisting() const {
    return WindowBackend::isExisting(data_);
  }

  void Window::forceExit() {
    WindowBackend::forceExit(data_);
  }

  void Window::swap() {
    XE_TRACE_BEGIN("XE", "Window swap");
    WindowBackend::swap(data_);
    XE_TRACE_END("XE", "Window swap");
  }

  void Window::stop() {
    WindowBackend::stop(data_);
  }

  Timestep Window::uptime() const {
    return Timestep(static_cast<float>(WindowBackend::uptime(data_)));
  }

  void Window::setSize(uint width, uint height) {
    data_->width = width;
    data_->height = height;
    WindowBackend::setSize(data_);
  }

  void Window::setTitle(const string &title) {
    data_->title = title;
    WindowBackend::setTitle(data_);
  }

  void Window::setIcon(uint width, uint height, byte *pixels) {
    data_->icon.width = width;
    data_->icon.height = height;
    data_->icon.pixels = pixels;
    WindowBackend::setIcon(data_);
  }

  void Window::setSwapInterval(bool enabled) {
    data_->swapInterval = enabled;
    WindowBackend::setSwapInterval(data_);
  }

  bool Window::pollEvent(Event &event) {
    if (data_->events.empty()) {
      XE_TRACE_BEGIN("XE", "Window poll events");
      WindowBackend::pollEvents(data_);
      XE_TRACE_END("XE", "Window poll events");
    } else {
      event = data_->events.front();
      data_->events.pop();

      if (event.type == Event::Resized) {
        setSize(event.size.width, event.size.height);
      }
      return true;
    }
    return false;
  }

  bool Window::isKeyPressed(Keyboard::Key key) {
    return WindowBackend::isKeyPressed(data_, key);
  }

  bool Window::isMouseButtonPressed(Mouse::Button button) {
    return WindowBackend::isMouseButtonPressed(data_, button);
  }

  vec2 Window::getMousePosition() {
    return WindowBackend::getMousePosition(data_);
  }

  void Window::setMousePosition(const vec2 &position) {
    WindowBackend::setMousePosition(data_, position);
  }

}
