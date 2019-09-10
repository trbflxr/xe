//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GL_WINDOW_HPP
#define XE_GL_WINDOW_HPP

#include "gl_window_data.hpp"

namespace xe {

  struct WindowBackend {
    static void init(Window::Data *data);
    static void initContext(Window::Data *data);
    static void swap(Window::Data *data);
    static void pollEvents(Window::Data *data);
    static void stop(Window::Data *data);

    static double uptime(Window::Data *data);

    static bool isExisting(Window::Data *data);
    static void forceExit(Window::Data *data);

    static void setSize(Window::Data *data);
    static void setTitle(Window::Data *data);
    static void setIcon(Window::Data *data);
    static void setSwapInterval(Window::Data *data);

    static bool isKeyPressed(Window::Data *data, Keyboard::Key key);
    static bool isMouseButtonPressed(Window::Data *data, Mouse::Button button);

    static vec2 getMousePosition(Window::Data *data);
    static void setMousePosition(Window::Data *data, const vec2 &position);
  };

}

#endif //XE_GL_WINDOW_HPP
