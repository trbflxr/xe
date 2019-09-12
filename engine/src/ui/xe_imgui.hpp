//
// Created by FLXR on 9/12/2019.
//

#ifndef XE_XE_IMGUI_HPP
#define XE_XE_IMGUI_HPP

#include <xe/graphics/window.hpp>
#include <xe/ui/ui.hpp>

namespace xe::ui {

  bool init(Window::Data *data);
  void update(Window &window);
  bool processEvent(const Event &e);
  void draw();
  void stop(Window::Data *data);

}

#endif //XE_XE_IMGUI_HPP
