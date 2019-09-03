//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GL_WINDOW_DATA_HPP
#define XE_GL_WINDOW_DATA_HPP

#include <atomic>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <xe/graphics/window.hpp>

namespace xe::detail {

  struct WindowData {
    GLFWwindow *window;
    GLFWcursor *mouseCursor;
    GLFWimage icon;

    string title;

    std::atomic<bool> swapInterval;

    std::atomic<uint> width;
    std::atomic<uint> height;

    double time = 0.0;
  };

}

#endif //XE_GL_WINDOW_DATA_HPP
