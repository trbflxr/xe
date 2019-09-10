//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GL_WINDOW_DATA_HPP
#define XE_GL_WINDOW_DATA_HPP

#include <atomic>
#include <queue>
#include "events/event.hpp"
#include "external/glad/glad.h"
#include <GLFW/glfw3.h>
#include <xe/graphics/window.hpp>

namespace xe {

  struct Window::Data {
    GLFWwindow *window;
    GLFWcursor *mouseCursor; //todo: implement
    GLFWimage icon;

    string title;

    std::atomic<bool> swapInterval;

    std::atomic<uint> width;
    std::atomic<uint> height;

    std::queue<Event> events;
  };

}

#endif //XE_GL_WINDOW_DATA_HPP
