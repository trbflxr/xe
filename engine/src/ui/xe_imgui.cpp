//
// Created by FLXR on 9/12/2019.
//

#include "xepch.hpp"
#include "xe_imgui.hpp"

#ifdef XE_PLATFORM_GL
  #include "ui/opengl/gl_imgui.hpp"
#else
  #error only gl currently supported
#endif

namespace xe::ui {

  bool init(Window::Data *data) {
    XE_TRACE_BEGIN("XE", "UI Init");
    impl::init(data);
    XE_TRACE_END("XE", "UI Init");
    return true;
  }

  void update(Window &window, Window::Data *data) {
    XE_TRACE_BEGIN("XE", "UI Update");
    impl::update(window, data);
    ImGui::NewFrame();
    XE_TRACE_END("XE", "UI Update");
  }

  bool processEvent(const Event &e) {
    bool handled = impl::processEvent(e);
    return handled;
  }

  void draw() {
    XE_TRACE_BEGIN("XE", "UI Draw");
    ImGui::Render();
    impl::draw(ImGui::GetDrawData());
    XE_TRACE_END("XE", "UI Draw");
  }

  void stop(Window::Data *data) {
    XE_TRACE_BEGIN("XE", "UI Stop");
    impl::stop(data);
    ImGui::DestroyContext();
    XE_TRACE_END("XE", "UI Stop");
  }

}
