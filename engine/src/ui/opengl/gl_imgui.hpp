//
// Created by FLXR on 9/12/2019.
//

#ifndef XE_GL_IMGUI_HPP
#define XE_GL_IMGUI_HPP

#include <xe/ui/imgui/imgui.h>
#include <xe/ui/imgui/imgui_internal.h>
#include "graphics/opengl/gl_window.hpp"

namespace xe::ui::impl {

  bool init(Window::Data *data);
  void update(Window &window, Window::Data *data);
  bool processEvent(const Event &e);
  void draw(ImDrawData *drawData);
  void stop(Window::Data *data);

  bool createFontTexture();
  void destroyFontTexture();
  bool createDeviceObjects();
  void destroyDeviceObjects();

}

#endif //XE_GL_IMGUI_HPP
