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

    struct Key {
      Keyboard::Key code;
      bool alt;
      bool control;
      bool shift;
      bool system;
    };

    struct Text {
      uint unicode;
    };

    struct MouseMove {
      float x;
      float y;
    };

    struct MouseButton {
      Mouse::Button button;
      bool alt;
      bool control;
      bool shift;
      bool system;
    };

    struct MouseWheel {
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
      Key key;
      Text text;
      MouseMove mouseMove;
      MouseButton mouseButton;
      MouseWheel mouseScroll;
    };
  };

}

#endif //XE_EVENT_HPP
