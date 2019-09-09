//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "gl_window.hpp"
#include <xe/utils/logger.hpp>

namespace xe {

  void WindowBackend::init(Window::Data *data) {
    if (!glfwInit()) {
      XE_CORE_CRITICAL("[WindowBackend / GL] Window failed at startup");
      return;
    }

    data->window = glfwCreateWindow(data->width, data->height, data->title.c_str(), nullptr, nullptr);

    if (!data->window) {
      XE_CORE_CRITICAL("[WindowBackend / GL] Could not create window");
      return;
    }

    glfwMakeContextCurrent(data->window);

    const int32 status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if (!status) {
      XE_CORE_CRITICAL("[WindowBackend / GL] Failed to initialize Glad");
      return;
    }

    setIcon(data);
    setSwapInterval(data);


    const byte *version = glGetString(GL_VERSION);
    const byte *vendor = glGetString(GL_VENDOR);
    const byte *renderer = glGetString(GL_RENDERER);

    XE_CORE_INFO("[WindowBackend / GL] Window initialized successful:\n"
                 "\t- glVersion \t\t({})\n"
                 "\t- glVendor \t\t({})\n"
                 "\t- glRenderer \t\t({})",
                 version, vendor, renderer);
  }

  void WindowBackend::swap(Window::Data *data) {
    glfwSwapBuffers(data->window);
  }

  void WindowBackend::pollEvents(Window::Data *data) {
    glfwPollEvents();
  }

  void WindowBackend::stop(Window::Data *data) {
    glfwTerminate();
  }

  double WindowBackend::uptime(Window::Data *data) {
    return glfwGetTime();
  }

  bool WindowBackend::isExisting(Window::Data *data) {
    return glfwWindowShouldClose(data->window);
  }

  void WindowBackend::forceExit(Window::Data *data) {
    glfwSetWindowShouldClose(data->window, GLFW_TRUE);
  }

  void WindowBackend::setTitle(Window::Data *data) {
    glfwSetWindowTitle(data->window, data->title.c_str());
  }

  void WindowBackend::setIcon(Window::Data *data) {
    if (!data->icon.pixels || !data->icon.width || !data->icon.height) {
      return;
    }
    glfwSetWindowIcon(data->window, 1, &data->icon);
  }

  void WindowBackend::setSwapInterval(Window::Data *data) {
    glfwSwapInterval(data->swapInterval);
  }

}
