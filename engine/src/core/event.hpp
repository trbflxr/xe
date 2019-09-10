//
// Created by FLXR on 9/10/2019.
//

#ifndef XE_EVENT_HPP
#define XE_EVENT_HPP

#include <xe/common.hpp>
#include <xe/core/input.hpp>

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
      bool alt;
      bool control;
      bool shift;
      bool system;
    };

    struct MouseWheelEvent {
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
      KeyRepeated,
      KeyReleased,
      MouseScrolled,
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
      MouseWheelEvent mouseScroll;
    };

    bool handled = false;
  };

}

#endif //XE_EVENT_HPP
