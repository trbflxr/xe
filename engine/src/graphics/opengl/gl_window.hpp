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

    static void setTitle(Window::Data *data);
    static void setIcon(Window::Data *data);
    static void setSwapInterval(Window::Data *data);
  };

}

#endif //XE_GL_WINDOW_HPP
