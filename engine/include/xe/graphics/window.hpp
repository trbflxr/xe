//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_WINDOW_HPP
#define XE_WINDOW_HPP

#include <functional>
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
    friend class Application;
  public:
    struct Data;

    Window();
    ~Window() override;

    void forceExit();

    vec2 framebufferSize() const;
    vec2 size() const;
    void setSize(uint width, uint height);

    void setTitle(const string &title);
    void setIcon(uint width, uint height, byte *pixels);
    void setSwapInterval(bool enabled);

    void setCursor(Cursor::Enum cursor) const;
    Cursor::Enum activeCursor() const;

    bool isCursorVisible() const;
    void setCursorVisible(bool visible) const;

    bool isExisting() const;
    Timestep uptime() const;

    inline Params::Window params() const { return params_; }
    inline void setParams(const Params::Window &params) { params_ = params; }

  protected:
    void init();
    void initContext();
    void update();
    void stop();

    bool pollEvent(Event &event);

    bool isKeyPressed(Keyboard::Key key);
    bool isMouseButtonPressed(Mouse::Button button);

    vec2 getMousePosition();
    void setMousePosition(const vec2 &position);

  protected:
    Data *data_;
    Params::Window params_{ };
  };

}

#endif //XE_WINDOW_HPP
