//
// Created by FLXR on 9/10/2019.
//

#ifndef XE_EVENT_HPP
#define XE_EVENT_HPP

#include <xe/input/keyboard.hpp>
#include <xe/input/mouse.hpp>

namespace xe {

  struct Event {
    struct SizeEvent {
      uint width;
      uint height;
    };

    struct KeyEvent {
      Keyboard::Key code;
      bool alt;
      bool control;
      bool shift;
      bool system;
    };

    struct TextEvent {
      uint unicode;
    };

    struct MouseMoveEvent {
      float x;
      float y;
    };

    struct MouseButtonEvent {
      Mouse::Button button;
      float x;
      float y;
    };

    struct MouseWheelEvent {
      int32 delta;
      float x;
      float y;
    };


    enum EventType {
      Closed,
      Resized,
      LostFocus,
      GainedFocus,
      TextEntered,
      KeyPressed,
      KeyReleased,
      MouseWheelMoved,
      MouseButtonPressed,
      MouseButtonReleased,
      MouseMoved,
      MouseEntered,
      MouseLeft,

      Count
    };

    EventType type;

    union {
      SizeEvent size;
      KeyEvent key;
      TextEvent text;
      MouseMoveEvent mouseMove;
      MouseButtonEvent mouseButton;
      MouseWheelEvent mouseWheel;
    };

    bool handled = false;
  };

}

#endif //XE_EVENT_HPP
