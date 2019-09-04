//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "gl_window.hpp"
#include <xe/utils/logger.hpp>

namespace xe::window {

  void init(detail::WindowData *data) {
    if (!glfwInit()) {
      XE_CORE_CRITICAL("Window failed at startup");
      return;
    }

    data->window = glfwCreateWindow(data->width, data->height, data->title.c_str(), nullptr, nullptr);

    if (!data->window) {
      XE_CORE_CRITICAL("Could not create window");
      return;
    }

    glfwMakeContextCurrent(data->window);

    const int32 status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if (!status) {
      XE_CORE_CRITICAL("Failed to initialize Glad");
      return;
    }

    setIcon(data);
    setSwapInterval(data);

    XE_CORE_INFO("Window initialized successful");
  }

  void swap(detail::WindowData *data) {
    glfwSwapBuffers(data->window);
  }

  void pollEvents(detail::WindowData *data) {
    glfwPollEvents();
  }

  void stop(detail::WindowData *data) {
    glfwTerminate();
  }

  double uptime(detail::WindowData *data) {
    return glfwGetTime();
  }

  bool isExisting(detail::WindowData *data) {
    return glfwWindowShouldClose(data->window);
  }

  void forceExit(detail::WindowData *data) {
    glfwSetWindowShouldClose(data->window, GLFW_TRUE);
  }

  void setTitle(detail::WindowData *data) {
    glfwSetWindowTitle(data->window, data->title.c_str());
  }

  void setIcon(detail::WindowData *data) {
    if (!data->icon.pixels || !data->icon.width || !data->icon.height) {
      return;
    }
    glfwSetWindowIcon(data->window, 1, &data->icon);
  }

  void setSwapInterval(detail::WindowData *data) {
    glfwSwapInterval(data->swapInterval);
  }

}
