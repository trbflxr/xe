//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/graphics/window.hpp>
#include <xe/utils/debug_trace.hpp>
#include "ui/xe_imgui.hpp"

#ifdef XE_PLATFORM_GL
  #include "graphics/opengl/gl_window.hpp"
#else
  #error only gl currently supported
#endif

namespace xe {

  Window::Window() {
    setName("Window");
    data_ = new Data();
    data_->title = "XE Window";
  }

  Window::~Window() {
    delete data_;
  }

  void Window::init() {
    data_->width = params_.width;
    data_->height = params_.height;
    data_->title = params_.title;
    data_->swapInterval = params_.swapInterval;
    data_->icon.width = params_.iconWidth;
    data_->icon.height = params_.iconHeight;
    data_->icon.pixels = params_.iconPixels;

    data_->activeCursor = Cursor::Hand;
    data_->cursorVisible = true;

    WindowBackend::init(data_);

    ui::init(data_);
  }

  void Window::initContext() {
    WindowBackend::initContext(data_, params_.srgb);
  }

  bool Window::isExisting() const {
    return WindowBackend::isExisting(data_);
  }

  void Window::forceExit() {
    WindowBackend::forceExit(data_);
  }

  void Window::update() {
    ui::update(*this, data_);

    XE_TRACE_BEGIN("XE", "UI Function");


    static const uint flags = ImGuiWindowFlags_NoDocking |
                              ImGuiWindowFlags_MenuBar |
                              ImGuiWindowFlags_NoTitleBar |
                              ImGuiWindowFlags_NoResize |
                              ImGuiWindowFlags_NoCollapse |
                              ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoSavedSettings |
                              ImGuiWindowFlags_NoFocusOnAppearing |
                              ImGuiWindowFlags_NoNav;


    const float DISTANCE = 10.0f;
    static int32 corner = 0;

    static bool showDemo = false;

    if (corner != -1) {
      const ImGuiViewport *viewport = ImGui::GetMainViewport();
      const ImVec2 windowPos = ImVec2(
          (corner & 1) ? (viewport->Pos.x + viewport->Size.x - DISTANCE) : (viewport->Pos.x + DISTANCE),
          (corner & 2) ? (viewport->Pos.y + viewport->Size.y - DISTANCE) : (viewport->Pos.y + DISTANCE));

      const ImVec2 windowPosPivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
      ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPosPivot);
      ImGui::SetNextWindowViewport(viewport->ID);
    }

    ImGui::SetNextWindowBgAlpha(0.3f);
    if (ImGui::Begin("MainMenu", nullptr, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | flags)) {
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Examples")) {

          if (ImGui::MenuItem("Demo", nullptr)) {
            showDemo = !showDemo;
          }
          if (ImGui::MenuItem("2D Particles", nullptr)) {

          }
          ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
      }
    }

    ImGui::Text("fps: %i", 60);
    ImGui::Text("tick rate: %i", 30);
    ImGui::Text("delta: %.4f", ImGui::GetIO().DeltaTime);
    ImGui::Text("uptime: %.2f", uptime().seconds());
    ImGui::Separator();
    ImGui::Dummy({10.0f, 0.0f});

    char buff[2048];
    memset(buff, 0, 2048);
    ImGui::InputText("Test", buff, 2048);
    ImGui::Dummy({10.0f, 0.0f});

    ImGui::Text("Right-click to change position");

    if (ImGui::BeginPopupContextWindow()) {
      if (ImGui::MenuItem("Custom", nullptr, corner == -1)) corner = -1;
      if (ImGui::MenuItem("Top-left", nullptr, corner == 0)) corner = 0;
      if (ImGui::MenuItem("Top-right", nullptr, corner == 1)) corner = 1;
      if (ImGui::MenuItem("Bottom-left", nullptr, corner == 2)) corner = 2;
      if (ImGui::MenuItem("Bottom-right", nullptr, corner == 3)) corner = 3;
      ImGui::EndPopup();
    }
    ImGui::End();


    if (showDemo) {
      ImGui::ShowDemoWindow(&showDemo);
    }

    XE_TRACE_END("XE", "UI Function");

    ui::draw();

    XE_TRACE_BEGIN("XE", "Window swap");
    WindowBackend::swap(data_);
    XE_TRACE_END("XE", "Window swap");
  }

  void Window::stop() {
    ui::stop(data_);

    WindowBackend::stop(data_);
  }

  Timestep Window::uptime() const {
    return Timestep(static_cast<float>(WindowBackend::uptime(data_)));
  }

  vec2 Window::framebufferSize() const {
    return WindowBackend::framebufferSize(data_);
  }

  vec2 Window::size() const {
    return vec2(static_cast<float>(data_->width), static_cast<float>(data_->height));
  }

  void Window::setSize(uint width, uint height) {
    data_->width = width;
    data_->height = height;
    WindowBackend::setSize(data_);
  }

  void Window::setTitle(const string &title) {
    data_->title = title;
    WindowBackend::setTitle(data_);
  }

  void Window::setIcon(uint width, uint height, byte *pixels) {
    data_->icon.width = width;
    data_->icon.height = height;
    data_->icon.pixels = pixels;
    WindowBackend::setIcon(data_);
  }

  void Window::setSwapInterval(bool enabled) {
    data_->swapInterval = enabled;
    WindowBackend::setSwapInterval(data_);
  }

  void Window::setCursor(Cursor::Enum cursor) const {
    WindowBackend::setCursor(data_, cursor);
  }

  Cursor::Enum Window::activeCursor() const {
    return data_->activeCursor;
  }

  bool Window::isCursorVisible() const {
    return data_->cursorVisible;
  }

  void Window::setCursorVisible(bool visible) const {
    WindowBackend::setCursorVisible(data_, visible);
  }

  void Window::Data::pushEvent(const Event &e) {
    //dispatch event to ui and if it's not handled push it to queue
    if (!ui::processEvent(e)) {
      events.push(e);
    }
  }

  bool Window::pollEvent(Event &event) {
    if (data_->events.empty()) {
      XE_TRACE_BEGIN("XE", "Window poll events");
      WindowBackend::pollEvents(data_);
      XE_TRACE_END("XE", "Window poll events");
    } else {
      event = data_->events.front();
      data_->events.pop();

      if (event.type == Event::Resized) {
        data_->width = event.size.width;
        data_->height = event.size.height;
      }
      return true;
    }
    return false;
  }

  bool Window::isKeyPressed(Keyboard::Key key) {
    return WindowBackend::isKeyPressed(data_, key);
  }

  bool Window::isMouseButtonPressed(Mouse::Button button) {
    return WindowBackend::isMouseButtonPressed(data_, button);
  }

  vec2 Window::getMousePosition() {
    return WindowBackend::getMousePosition(data_);
  }

  void Window::setMousePosition(const vec2 &position) {
    WindowBackend::setMousePosition(data_, position);
  }

}
