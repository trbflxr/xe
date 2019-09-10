//
// Created by FLXR on 9/10/2019.
//

#include "xepch.hpp"
#include <glfw/glfw3.h>
#include <xe/input/keyboard.hpp>
#include <xe/input/mouse.hpp>

namespace xe {

  bool Keyboard::isKeyPressed(Keyboard::Key key) {
    int32 glfwKey = 0;
    switch (key) {
      case A: break;
      case B: break;
      case C: break;
      case D: break;
      case E: break;
      case F: break;
      case G: break;
      case H: break;
      case I: break;
      case J: break;
      case K: break;
      case L: break;
      case M: break;
      case N: break;
      case O: break;
      case P: break;
      case Q: break;
      case R: break;
      case S: break;
      case T: break;
      case U: break;
      case V: break;
      case W: break;
      case X: break;
      case Y: break;
      case Z: break;
      case Num0: break;
      case Num1: break;
      case Num2: break;
      case Num3: break;
      case Num4: break;
      case Num5: break;
      case Num6: break;
      case Num7: break;
      case Num8: break;
      case Num9: break;
      case Escape: break;
      case LControl: break;
      case LShift: break;
      case LAlt: break;
      case LSystem: break;
      case RControl: break;
      case RShift: break;
      case RAlt: break;
      case RSystem: break;
      case Menu: break;
      case LBracket: break;
      case RBracket: break;
      case SemiColon: break;
      case Comma: break;
      case Period: break;
      case Quote: break;
      case Slash: break;
      case BackSlash: break;
      case Tilde: break;
      case Equal: break;
      case Dash: break;
      case Space: break;
      case Return: break;
      case BackSpace: break;
      case Tab: break;
      case PageUp: break;
      case PageDown: break;
      case End: break;
      case Home: break;
      case Insert: break;
      case Delete: break;
      case Add: break;
      case Subtract: break;
      case Multiply: break;
      case Divide: break;
      case Left: break;
      case Right: break;
      case Up: break;
      case Down: break;
      case Numpad0: break;
      case Numpad1: break;
      case Numpad2: break;
      case Numpad3: break;
      case Numpad4: break;
      case Numpad5: break;
      case Numpad6: break;
      case Numpad7: break;
      case Numpad8: break;
      case Numpad9: break;
      case F1: break;
      case F2: break;
      case F3: break;
      case F4: break;
      case F5: break;
      case F6: break;
      case F7: break;
      case F8: break;
      case F9: break;
      case F10: break;
      case F11: break;
      case F12: break;
      case F13: break;
      case F14: break;
      case F15: break;
      case Pause: break;
      case KeyCount: break;
      default: break;
    }
    return false;
//    glfwGetKey()
  }

  bool Mouse::isButtonPressed(Mouse::Button button) {
    return false;
  }

  vec2 Mouse::getPosition() {
    return vec2();
  }

  vec2 Mouse::getPosition(const Window &relativeTo) {
    return vec2();
  }

  void Mouse::setPosition(const vec2 &position) {

  }

  void Mouse::setPosition(const vec2 &position, const Window &relativeTo) {

  }

}
