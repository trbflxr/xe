//
// Created by FLXR on 9/10/2019.
//

#ifndef XE_MOUSE_HPP
#define XE_MOUSE_HPP

#include <xe/math/vec2.hpp>
#include <xe/common.hpp>

namespace xe {

  class Window;

  struct XE_API Mouse {
    enum Button {
      Left,
      Right,
      Middle,
      XButton1,
      XButton2,

      ButtonCount
    };

    static bool isButtonPressed(Button button);

    static vec2 getPosition();
    static vec2 getPosition(const Window &relativeTo);

    static void setPosition(const vec2 &position);
    static void setPosition(const vec2 &position, const Window &relativeTo);
  };

}

#endif //XE_MOUSE_HPP
