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
    data_ = new detail::WindowData();
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

    window::init(data_);
  }

  bool Window::isExisting() const {
    return window::isExisting(data_);
  }

  void Window::forceExit() {
    window::forceExit(data_);
  }

  void Window::pollEvents() {
    XE_TRACE_BEGIN("XE", "Window poll events");
    window::pollEvents(data_);
    XE_TRACE_BEGIN("XE", "Window poll events");
  }

  void Window::swap() {
    XE_TRACE_BEGIN("XE", "Window swap");
    window::swap(data_);
    XE_TRACE_END("XE", "Window swap");
  }

  void Window::stop() {
    window::stop(data_);
  }

  double Window::uptime() const {
    return window::uptime(data_);
  }

  void Window::setTitle(const string &title) {
    data_->title = title;
    window::setTitle(data_);
  }

  void Window::setIcon(uint width, uint height, byte *pixels) {
    data_->icon.width = width;
    data_->icon.height = height;
    data_->icon.pixels = pixels;
    window::setIcon(data_);
  }

  void Window::setSwapInterval(bool enabled) {
    data_->swapInterval = enabled;
    window::setSwapInterval(data_);
  }

}
