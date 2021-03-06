//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_WINDOW_HPP
#define XE_WINDOW_HPP

#include <functional>
#include <xe/params.hpp>
#include <xe/core/event.hpp>
#include <xe/core/object.hpp>
#include <xe/core/timestep.hpp>

namespace xe {

  //todo: create cursor system
  struct Cursor {
    enum Enum {
      Arrow,
      IBeam,
      Crosshair,
      Hand,
      ResizeX,
      ResizeY,

      Count
    };
  };

  //todo: create more getters/setters
  //todo: fix icon setter
  class XE_API Window : public Object {
  XE_OBJECT(Window, Object);
    friend class GPU;
    friend class Engine;
  public:
    struct Data;

    Window();
    ~Window() override;

    vec2u framebufferSize() const;
    vec2u size() const;
    void setSize(const vec2u &size);

    std::string_view getTitle() const;
    void setTitle(std::string title);

    bool isVsyncEnabled() const;
    void setVsyncEnabled(bool enabled);

    void setIcon(uint32_t width, uint32_t height, uint8_t *pixels);

    void setCursor(Cursor::Enum cursor) const;
    Cursor::Enum activeCursor() const;

    bool isCursorVisible() const;
    void setCursorVisible(bool visible) const;

    bool shouldClose() const;
    Timestep uptime() const;

  private:
    void init();
    void update();
    void stop();

    bool pollEvent(Event &event);

    void forceExit();

    bool isKeyPressed(Keyboard::Key key);
    bool isMouseButtonPressed(Mouse::Button button);

    vec2 getMousePosition();
    void setMousePosition(const vec2 &position);

  private:
    Data *data_;
    std::function<void(void *)> ui_ = [](void *) { };
    void *uiData_ = nullptr;
    Params::Window params_{ };
  };

}

#endif //XE_WINDOW_HPP
