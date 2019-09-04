//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GL_WINDOW_HPP
#define XE_GL_WINDOW_HPP

#include "gl_window_data.hpp"

namespace xe::window {

  void init(detail::WindowData *data);
  void swap(detail::WindowData *data);
  void pollEvents(detail::WindowData *data);
  void stop(detail::WindowData *data);

  double uptime(detail::WindowData *data);

  bool isExisting(detail::WindowData *data);
  void forceExit(detail::WindowData *data);

  void setTitle(detail::WindowData *data);
  void setIcon(detail::WindowData *data);
  void setSwapInterval(detail::WindowData *data);

}

#endif //XE_GL_WINDOW_HPP
