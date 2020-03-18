//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "gl_window.hpp"

#include <xe/utils/logger.hpp>

namespace xe {

  static Keyboard::Key glfwKeyToXe(int32_t key) {
    switch (key) {
      case GLFW_KEY_A: return Keyboard::A;
      case GLFW_KEY_B: return Keyboard::B;
      case GLFW_KEY_C: return Keyboard::C;
      case GLFW_KEY_D: return Keyboard::D;
      case GLFW_KEY_E: return Keyboard::E;
      case GLFW_KEY_F: return Keyboard::F;
      case GLFW_KEY_G: return Keyboard::G;
      case GLFW_KEY_H: return Keyboard::H;
      case GLFW_KEY_I: return Keyboard::I;
      case GLFW_KEY_J: return Keyboard::J;
      case GLFW_KEY_K: return Keyboard::K;
      case GLFW_KEY_L: return Keyboard::L;
      case GLFW_KEY_M: return Keyboard::M;
      case GLFW_KEY_N: return Keyboard::N;
      case GLFW_KEY_O: return Keyboard::O;
      case GLFW_KEY_P: return Keyboard::P;
      case GLFW_KEY_Q: return Keyboard::Q;
      case GLFW_KEY_R: return Keyboard::R;
      case GLFW_KEY_S: return Keyboard::S;
      case GLFW_KEY_T: return Keyboard::T;
      case GLFW_KEY_U: return Keyboard::U;
      case GLFW_KEY_V: return Keyboard::V;
      case GLFW_KEY_W: return Keyboard::W;
      case GLFW_KEY_X: return Keyboard::X;
      case GLFW_KEY_Y: return Keyboard::Y;
      case GLFW_KEY_Z: return Keyboard::Z;
      case GLFW_KEY_0: return Keyboard::Num0;
      case GLFW_KEY_1: return Keyboard::Num1;
      case GLFW_KEY_2: return Keyboard::Num2;
      case GLFW_KEY_3: return Keyboard::Num3;
      case GLFW_KEY_4: return Keyboard::Num4;
      case GLFW_KEY_5: return Keyboard::Num5;
      case GLFW_KEY_6: return Keyboard::Num6;
      case GLFW_KEY_7: return Keyboard::Num7;
      case GLFW_KEY_8: return Keyboard::Num8;
      case GLFW_KEY_9: return Keyboard::Num9;
      case GLFW_KEY_SPACE: return Keyboard::Space;
      case GLFW_KEY_APOSTROPHE: return Keyboard::Quote;
      case GLFW_KEY_COMMA: return Keyboard::Comma;
      case GLFW_KEY_MINUS: return Keyboard::Dash;
      case GLFW_KEY_PERIOD: return Keyboard::Period;
      case GLFW_KEY_SLASH: return Keyboard::Slash;
      case GLFW_KEY_SEMICOLON: return Keyboard::SemiColon;
      case GLFW_KEY_EQUAL: return Keyboard::Equal;
      case GLFW_KEY_LEFT_BRACKET: return Keyboard::LBracket;
      case GLFW_KEY_BACKSLASH: return Keyboard::BackSlash;
      case GLFW_KEY_RIGHT_BRACKET: return Keyboard::RBracket;
      case GLFW_KEY_GRAVE_ACCENT: return Keyboard::Tilde;
      case GLFW_KEY_ESCAPE: return Keyboard::Escape;
      case GLFW_KEY_ENTER: return Keyboard::Return;
      case GLFW_KEY_TAB: return Keyboard::Tab;
      case GLFW_KEY_BACKSPACE: return Keyboard::BackSpace;
      case GLFW_KEY_INSERT: return Keyboard::Insert;
      case GLFW_KEY_DELETE: return Keyboard::Delete;
      case GLFW_KEY_RIGHT: return Keyboard::Right;
      case GLFW_KEY_LEFT: return Keyboard::Left;
      case GLFW_KEY_DOWN: return Keyboard::Down;
      case GLFW_KEY_UP: return Keyboard::Up;
      case GLFW_KEY_PAGE_UP: return Keyboard::PageUp;
      case GLFW_KEY_PAGE_DOWN: return Keyboard::PageDown;
      case GLFW_KEY_HOME: return Keyboard::Home;
      case GLFW_KEY_END: return Keyboard::End;
      case GLFW_KEY_CAPS_LOCK: return Keyboard::CapsLock;
      case GLFW_KEY_SCROLL_LOCK: return Keyboard::ScrollLock;
      case GLFW_KEY_NUM_LOCK: return Keyboard::NumLock;
      case GLFW_KEY_PRINT_SCREEN: return Keyboard::PrintScreen;
      case GLFW_KEY_PAUSE: return Keyboard::Pause;
      case GLFW_KEY_F1: return Keyboard::F1;
      case GLFW_KEY_F2: return Keyboard::F2;
      case GLFW_KEY_F3: return Keyboard::F3;
      case GLFW_KEY_F4: return Keyboard::F4;
      case GLFW_KEY_F5: return Keyboard::F5;
      case GLFW_KEY_F6: return Keyboard::F6;
      case GLFW_KEY_F7: return Keyboard::F7;
      case GLFW_KEY_F8: return Keyboard::F8;
      case GLFW_KEY_F9: return Keyboard::F9;
      case GLFW_KEY_F10: return Keyboard::F10;
      case GLFW_KEY_F11: return Keyboard::F11;
      case GLFW_KEY_F12: return Keyboard::F12;
      case GLFW_KEY_F13: return Keyboard::F13;
      case GLFW_KEY_F14: return Keyboard::F14;
      case GLFW_KEY_F15: return Keyboard::F15;
      case GLFW_KEY_KP_0: return Keyboard::Numpad0;
      case GLFW_KEY_KP_1: return Keyboard::Numpad1;
      case GLFW_KEY_KP_2: return Keyboard::Numpad2;
      case GLFW_KEY_KP_3: return Keyboard::Numpad3;
      case GLFW_KEY_KP_4: return Keyboard::Numpad4;
      case GLFW_KEY_KP_5: return Keyboard::Numpad5;
      case GLFW_KEY_KP_6: return Keyboard::Numpad6;
      case GLFW_KEY_KP_7: return Keyboard::Numpad7;
      case GLFW_KEY_KP_8: return Keyboard::Numpad8;
      case GLFW_KEY_KP_9: return Keyboard::Numpad9;
      case GLFW_KEY_KP_DECIMAL: return Keyboard::NumpadDecimal;
      case GLFW_KEY_KP_DIVIDE: return Keyboard::Divide;
      case GLFW_KEY_KP_MULTIPLY: return Keyboard::Multiply;
      case GLFW_KEY_KP_SUBTRACT: return Keyboard::Subtract;
      case GLFW_KEY_KP_ADD: return Keyboard::Add;
      case GLFW_KEY_KP_ENTER: return Keyboard::NumpadReturn;
      case GLFW_KEY_KP_EQUAL: return Keyboard::Equal;
      case GLFW_KEY_LEFT_SHIFT: return Keyboard::LShift;
      case GLFW_KEY_LEFT_CONTROL: return Keyboard::LControl;
      case GLFW_KEY_LEFT_ALT: return Keyboard::LAlt;
      case GLFW_KEY_LEFT_SUPER: return Keyboard::LSystem;
      case GLFW_KEY_RIGHT_SHIFT: return Keyboard::RShift;
      case GLFW_KEY_RIGHT_CONTROL: return Keyboard::RControl;
      case GLFW_KEY_RIGHT_ALT: return Keyboard::RAlt;
      case GLFW_KEY_RIGHT_SUPER: return Keyboard::RSystem;
      case GLFW_KEY_MENU: return Keyboard::Menu;
      default: break;
    }
    return Keyboard::Unknown;
  }

  static Mouse::Button glfwButtonToXe(int32_t button) {
    switch (button) {
      case GLFW_MOUSE_BUTTON_1: return Mouse::Left;
      case GLFW_MOUSE_BUTTON_2: return Mouse::Right;
      case GLFW_MOUSE_BUTTON_3: return Mouse::Middle;
      case GLFW_MOUSE_BUTTON_4: return Mouse::Button4;
      case GLFW_MOUSE_BUTTON_5: return Mouse::Button5;
      case GLFW_MOUSE_BUTTON_6: return Mouse::Button6;
      case GLFW_MOUSE_BUTTON_7: return Mouse::Button7;
      case GLFW_MOUSE_BUTTON_8: return Mouse::Button8;
      default: break;
    }
    return Mouse::ButtonCount;
  }

  void WindowBackend::init(Window::Data *data) {
    if (!glfwInit()) {
      XE_CORE_CRITICAL("[WindowBackend / GL] Window failed at startup");
      return;
    }

    data->window = glfwCreateWindow(data->width, data->height, data->title.c_str(), nullptr, nullptr);

    if (!data->window) {
      XE_CORE_CRITICAL("[WindowBackend / GL] Could not create window");
      return;
    }

    data->defaultCursors[Cursor::Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    data->defaultCursors[Cursor::IBeam] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    data->defaultCursors[Cursor::Crosshair] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    data->defaultCursors[Cursor::Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    data->defaultCursors[Cursor::ResizeX] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    data->defaultCursors[Cursor::ResizeY] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

    glfwSetCursor(data->window, data->defaultCursors[data->activeCursor]);

    glfwSetWindowUserPointer(data->window, data);

    glfwSetWindowCloseCallback(data->window, [](GLFWwindow *window) {
      auto &&data = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));

      Event e{ };
      e.type = Event::Closed;
      data->pushEvent(e);
    });

    glfwSetKeyCallback(data->window, [](GLFWwindow *window, int32_t key, int32_t /*scanCode*/, int32_t action, int32_t mods) {
      auto &&data = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));

      Event e{ };
      if (action == GLFW_PRESS) {
        e.type = Event::KeyPressed;
      } else if (action == GLFW_REPEAT) {
        e.type = Event::KeyRepeated;
      } else if (action == GLFW_RELEASE) {
        e.type = Event::KeyReleased;
      }
      e.key.code = glfwKeyToXe(key);
      e.key.shift = mods == GLFW_MOD_SHIFT;
      e.key.control = mods == GLFW_MOD_CONTROL;
      e.key.alt = mods == GLFW_MOD_ALT;
      e.key.system = mods == GLFW_MOD_SUPER;
      data->pushEvent(e);
    });

    glfwSetMouseButtonCallback(data->window, [](GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
      auto &&data = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));

      Event e{ };
      if (action == GLFW_PRESS) {
        e.type = Event::MouseButtonPressed;
      } else if (action == GLFW_RELEASE) {
        e.type = Event::MouseButtonReleased;
      }
      e.mouseButton.button = glfwButtonToXe(button);
      e.mouseButton.shift = mods == GLFW_MOD_SHIFT;
      e.mouseButton.control = mods == GLFW_MOD_CONTROL;
      e.mouseButton.alt = mods == GLFW_MOD_ALT;
      e.mouseButton.system = mods == GLFW_MOD_SUPER;
      data->pushEvent(e);
    });

    glfwSetScrollCallback(data->window, [](GLFWwindow *window, double xOffset, double yOffset) {
      auto &&data = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));

      Event e{ };
      e.type = Event::MouseScrolled;
      e.mouseScroll.x = static_cast<float>(xOffset);
      e.mouseScroll.y = static_cast<float>(yOffset);
      data->pushEvent(e);
    });

    glfwSetCursorPosCallback(data->window, [](GLFWwindow *window, double xPos, double yPos) {
      auto &&data = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));

      Event e{ };
      e.type = Event::MouseMoved;
      e.mouseMove.x = static_cast<float>(xPos);
      e.mouseMove.y = static_cast<float>(yPos);
      data->pushEvent(e);
    });

    glfwSetCharCallback(data->window, [](GLFWwindow *window, uint32_t ch) {
      auto &&data = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));

      Event e{ };
      e.type = Event::TextEntered;
      e.text.unicode = ch;
      data->pushEvent(e);
    });

    glfwSetWindowSizeCallback(data->window, [](GLFWwindow *window, int32_t width, int32_t height) {
      auto &&data = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));

      Event e{ };
      e.type = Event::Resized;
      e.size.width = static_cast<uint32_t>(width);
      e.size.height = static_cast<uint32_t>(height);
      data->pushEvent(e);
    });

    glfwSetCursorEnterCallback(data->window, [](GLFWwindow *window, int32_t entered) {
      auto &&data = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));

      Event e{ };
      e.type = entered ? Event::MouseEntered : Event::MouseLeft;
      data->pushEvent(e);
    });

    glfwSetWindowFocusCallback(data->window, [](GLFWwindow *window, int32_t focused) {
      auto &&data = static_cast<Window::Data *>(glfwGetWindowUserPointer(window));

      Event e{ };
      e.type = focused ? Event::GainedFocus : Event::LostFocus;
      data->pushEvent(e);
    });
  }

  void WindowBackend::initContext(Window::Data *data, bool srgb) {
    glfwMakeContextCurrent(data->window);

    const int32_t status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if (!status) {
      XE_CORE_CRITICAL("[WindowBackend / GL] Failed to initialize Glad");
      return;
    }

    setIcon(data);
    setSwapInterval(data);

    if (srgb) {
      glEnable(GL_FRAMEBUFFER_SRGB);
    } else {
      glDisable(GL_FRAMEBUFFER_SRGB);
    }

    const uint8_t *version = glGetString(GL_VERSION);
    const uint8_t *vendor = glGetString(GL_VENDOR);
    const uint8_t *renderer = glGetString(GL_RENDERER);

    XE_CORE_INFO("[WindowBackend / GL] Window initialized successful:\n"
                 "\t- glVersion \t\t({})\n"
                 "\t- glVendor \t\t({})\n"
                 "\t- glRenderer \t\t({})\n"
                 "\t- glSrgb \t\t({})",
                 version, vendor, renderer, srgb);
  }

  void WindowBackend::swap(Window::Data *data) {
    glfwSwapBuffers(data->window);
  }

  void WindowBackend::pollEvents(Window::Data * /*data*/) {
    glfwPollEvents();
  }

  void WindowBackend::stop(Window::Data *data) {
    for (auto &&c : data->defaultCursors) {
      glfwDestroyCursor(c);
    }
    glfwTerminate();
  }

  double WindowBackend::uptime(Window::Data * /*data*/) {
    return glfwGetTime();
  }

  bool WindowBackend::isExisting(Window::Data *data) {
    return glfwWindowShouldClose(data->window);
  }

  void WindowBackend::forceExit(Window::Data *data) {
    glfwSetWindowShouldClose(data->window, GLFW_TRUE);
  }

  vec2u WindowBackend::framebufferSize(Window::Data *data) {
    int32_t w = 0;
    int32_t h = 0;
    glfwGetFramebufferSize(data->window, &w, &h);
    return vec2u(static_cast<uint32_t>(w), static_cast<uint32_t>(h));
  }

  void WindowBackend::setSize(Window::Data *data) {
    glfwSetWindowSize(data->window, static_cast<int32_t>(data->width), static_cast<int32_t>(data->height));
  }

  void WindowBackend::setTitle(Window::Data *data) {
    glfwSetWindowTitle(data->window, data->title.c_str());
  }

  void WindowBackend::setIcon(Window::Data *data) {
    if (!data->icon.pixels || !data->icon.width || !data->icon.height) {
      return;
    }
    glfwSetWindowIcon(data->window, 1, &data->icon);
  }

  void WindowBackend::setSwapInterval(Window::Data *data) {
    glfwSwapInterval(data->swapInterval);
  }

  void WindowBackend::setCursor(Window::Data *data, Cursor::Enum cursor) {
    data->activeCursor = cursor;
    glfwSetCursor(data->window, data->defaultCursors[cursor]);
  }

  void WindowBackend::setCursorVisible(Window::Data *data, bool visible) {
    data->cursorVisible = visible;
    glfwSetInputMode(data->window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
  }

  bool WindowBackend::isKeyPressed(Window::Data *data, Keyboard::Key key) {
    int32_t glfwCode = 0;
    switch (key) {
      case Keyboard::A: glfwCode = GLFW_KEY_A;
        break;
      case Keyboard::B: glfwCode = GLFW_KEY_B;
        break;
      case Keyboard::C: glfwCode = GLFW_KEY_C;
        break;
      case Keyboard::D: glfwCode = GLFW_KEY_D;
        break;
      case Keyboard::E: glfwCode = GLFW_KEY_E;
        break;
      case Keyboard::F: glfwCode = GLFW_KEY_F;
        break;
      case Keyboard::G: glfwCode = GLFW_KEY_G;
        break;
      case Keyboard::H: glfwCode = GLFW_KEY_H;
        break;
      case Keyboard::I: glfwCode = GLFW_KEY_I;
        break;
      case Keyboard::J: glfwCode = GLFW_KEY_J;
        break;
      case Keyboard::K: glfwCode = GLFW_KEY_K;
        break;
      case Keyboard::L: glfwCode = GLFW_KEY_L;
        break;
      case Keyboard::M: glfwCode = GLFW_KEY_M;
        break;
      case Keyboard::N: glfwCode = GLFW_KEY_N;
        break;
      case Keyboard::O: glfwCode = GLFW_KEY_O;
        break;
      case Keyboard::P: glfwCode = GLFW_KEY_P;
        break;
      case Keyboard::Q: glfwCode = GLFW_KEY_Q;
        break;
      case Keyboard::R: glfwCode = GLFW_KEY_R;
        break;
      case Keyboard::S: glfwCode = GLFW_KEY_S;
        break;
      case Keyboard::T: glfwCode = GLFW_KEY_T;
        break;
      case Keyboard::U: glfwCode = GLFW_KEY_U;
        break;
      case Keyboard::V: glfwCode = GLFW_KEY_V;
        break;
      case Keyboard::W: glfwCode = GLFW_KEY_W;
        break;
      case Keyboard::X: glfwCode = GLFW_KEY_X;
        break;
      case Keyboard::Y: glfwCode = GLFW_KEY_Y;
        break;
      case Keyboard::Z: glfwCode = GLFW_KEY_Z;
        break;
      case Keyboard::Num0: glfwCode = GLFW_KEY_0;
        break;
      case Keyboard::Num1: glfwCode = GLFW_KEY_1;
        break;
      case Keyboard::Num2: glfwCode = GLFW_KEY_2;
        break;
      case Keyboard::Num3: glfwCode = GLFW_KEY_3;
        break;
      case Keyboard::Num4: glfwCode = GLFW_KEY_4;
        break;
      case Keyboard::Num5: glfwCode = GLFW_KEY_5;
        break;
      case Keyboard::Num6: glfwCode = GLFW_KEY_6;
        break;
      case Keyboard::Num7: glfwCode = GLFW_KEY_7;
        break;
      case Keyboard::Num8: glfwCode = GLFW_KEY_8;
        break;
      case Keyboard::Num9: glfwCode = GLFW_KEY_9;
        break;
      case Keyboard::Escape: glfwCode = GLFW_KEY_ESCAPE;
        break;
      case Keyboard::LControl: glfwCode = GLFW_KEY_LEFT_CONTROL;
        break;
      case Keyboard::LShift: glfwCode = GLFW_KEY_LEFT_SHIFT;
        break;
      case Keyboard::LAlt: glfwCode = GLFW_KEY_LEFT_ALT;
        break;
      case Keyboard::LSystem: glfwCode = GLFW_KEY_LEFT_SUPER;
        break;
      case Keyboard::RControl: glfwCode = GLFW_KEY_RIGHT_CONTROL;
        break;
      case Keyboard::RShift: glfwCode = GLFW_KEY_RIGHT_SHIFT;
        break;
      case Keyboard::RAlt: glfwCode = GLFW_KEY_RIGHT_ALT;
        break;
      case Keyboard::RSystem: glfwCode = GLFW_KEY_RIGHT_SUPER;
        break;
      case Keyboard::Menu: glfwCode = GLFW_KEY_MENU;
        break;
      case Keyboard::LBracket: glfwCode = GLFW_KEY_LEFT_BRACKET;
        break;
      case Keyboard::RBracket: glfwCode = GLFW_KEY_RIGHT_BRACKET;
        break;
      case Keyboard::SemiColon: glfwCode = GLFW_KEY_SEMICOLON;
        break;
      case Keyboard::Comma: glfwCode = GLFW_KEY_COMMA;
        break;
      case Keyboard::Period: glfwCode = GLFW_KEY_PERIOD;
        break;
      case Keyboard::Quote: glfwCode = GLFW_KEY_APOSTROPHE;
        break;
      case Keyboard::Slash: glfwCode = GLFW_KEY_SLASH;
        break;
      case Keyboard::BackSlash: glfwCode = GLFW_KEY_BACKSLASH;
        break;
      case Keyboard::Tilde: glfwCode = GLFW_KEY_GRAVE_ACCENT;
        break;
      case Keyboard::Equal: glfwCode = GLFW_KEY_EQUAL;
        break;
      case Keyboard::Dash: glfwCode = GLFW_KEY_MINUS;
        break;
      case Keyboard::Space: glfwCode = GLFW_KEY_SPACE;
        break;
      case Keyboard::Return: glfwCode = GLFW_KEY_ENTER;
        break;
      case Keyboard::BackSpace: glfwCode = GLFW_KEY_BACKSPACE;
        break;
      case Keyboard::Tab: glfwCode = GLFW_KEY_TAB;
        break;
      case Keyboard::PageUp: glfwCode = GLFW_KEY_PAGE_UP;
        break;
      case Keyboard::PageDown: glfwCode = GLFW_KEY_PAGE_DOWN;
        break;
      case Keyboard::End: glfwCode = GLFW_KEY_END;
        break;
      case Keyboard::Home: glfwCode = GLFW_KEY_HOME;
        break;
      case Keyboard::Insert: glfwCode = GLFW_KEY_INSERT;
        break;
      case Keyboard::Delete: glfwCode = GLFW_KEY_DELETE;
        break;
      case Keyboard::Add: glfwCode = GLFW_KEY_KP_ADD;
        break;
      case Keyboard::Subtract:glfwCode = GLFW_KEY_KP_SUBTRACT;
        break;
      case Keyboard::Multiply:glfwCode = GLFW_KEY_KP_MULTIPLY;
        break;
      case Keyboard::Divide: glfwCode = GLFW_KEY_KP_DIVIDE;
        break;
      case Keyboard::Left: glfwCode = GLFW_KEY_LEFT;
        break;
      case Keyboard::Right: glfwCode = GLFW_KEY_RIGHT;
        break;
      case Keyboard::Up: glfwCode = GLFW_KEY_UP;
        break;
      case Keyboard::Down: glfwCode = GLFW_KEY_DOWN;
        break;
      case Keyboard::CapsLock: glfwCode = GLFW_KEY_CAPS_LOCK;
        break;
      case Keyboard::ScrollLock: glfwCode = GLFW_KEY_SCROLL_LOCK;
        break;
      case Keyboard::NumLock: glfwCode = GLFW_KEY_NUM_LOCK;
        break;
      case Keyboard::PrintScreen: glfwCode = GLFW_KEY_PRINT_SCREEN;
        break;
      case Keyboard::Numpad0: glfwCode = GLFW_KEY_KP_0;
        break;
      case Keyboard::Numpad1: glfwCode = GLFW_KEY_KP_1;
        break;
      case Keyboard::Numpad2: glfwCode = GLFW_KEY_KP_2;
        break;
      case Keyboard::Numpad3: glfwCode = GLFW_KEY_KP_3;
        break;
      case Keyboard::Numpad4: glfwCode = GLFW_KEY_KP_4;
        break;
      case Keyboard::Numpad5: glfwCode = GLFW_KEY_KP_5;
        break;
      case Keyboard::Numpad6: glfwCode = GLFW_KEY_KP_6;
        break;
      case Keyboard::Numpad7: glfwCode = GLFW_KEY_KP_7;
        break;
      case Keyboard::Numpad8: glfwCode = GLFW_KEY_KP_8;
        break;
      case Keyboard::Numpad9: glfwCode = GLFW_KEY_KP_9;
        break;
      case Keyboard::NumpadDecimal: glfwCode = GLFW_KEY_KP_DECIMAL;
        break;
      case Keyboard::NumpadReturn: glfwCode = GLFW_KEY_KP_ENTER;
        break;
      case Keyboard::F1: glfwCode = GLFW_KEY_F1;
        break;
      case Keyboard::F2: glfwCode = GLFW_KEY_F2;
        break;
      case Keyboard::F3: glfwCode = GLFW_KEY_F3;
        break;
      case Keyboard::F4: glfwCode = GLFW_KEY_F4;
        break;
      case Keyboard::F5: glfwCode = GLFW_KEY_F5;
        break;
      case Keyboard::F6: glfwCode = GLFW_KEY_F6;
        break;
      case Keyboard::F7: glfwCode = GLFW_KEY_F7;
        break;
      case Keyboard::F8: glfwCode = GLFW_KEY_F8;
        break;
      case Keyboard::F9: glfwCode = GLFW_KEY_F9;
        break;
      case Keyboard::F10: glfwCode = GLFW_KEY_F10;
        break;
      case Keyboard::F11: glfwCode = GLFW_KEY_F11;
        break;
      case Keyboard::F12: glfwCode = GLFW_KEY_F12;
        break;
      case Keyboard::F13: glfwCode = GLFW_KEY_F13;
        break;
      case Keyboard::F14: glfwCode = GLFW_KEY_F14;
        break;
      case Keyboard::F15: glfwCode = GLFW_KEY_F15;
        break;
      case Keyboard::Pause: glfwCode = GLFW_KEY_PAUSE;
        break;
      default: break;
    }

    const int32_t state = glfwGetKey(data->window, glfwCode);
    return state == GLFW_PRESS;
  }

  bool WindowBackend::isMouseButtonPressed(Window::Data *data, Mouse::Button button) {
    int32_t glfwButton = 0;
    switch (button) {
      case Mouse::Left: glfwButton = GLFW_MOUSE_BUTTON_1;
        break;
      case Mouse::Right: glfwButton = GLFW_MOUSE_BUTTON_2;
        break;
      case Mouse::Middle: glfwButton = GLFW_MOUSE_BUTTON_3;
        break;
      case Mouse::Button4: glfwButton = GLFW_MOUSE_BUTTON_4;
        break;
      case Mouse::Button5: glfwButton = GLFW_MOUSE_BUTTON_5;
        break;
      case Mouse::Button6: glfwButton = GLFW_MOUSE_BUTTON_6;
        break;
      case Mouse::Button7: glfwButton = GLFW_MOUSE_BUTTON_7;
        break;
      case Mouse::Button8: glfwButton = GLFW_MOUSE_BUTTON_8;
        break;
      default: break;
    }

    const int32_t state = glfwGetMouseButton(data->window, glfwButton);
    return state == GLFW_PRESS;
  }

  vec2 WindowBackend::getMousePosition(Window::Data *data) {
    double x = 0.0;
    double y = 0.0;
    glfwGetCursorPos(data->window, &x, &y);
    return vec2(static_cast<float>(x), static_cast<float>(y));
  }

  void WindowBackend::setMousePosition(Window::Data *data, const vec2 &position) {
    glfwSetCursorPos(data->window, position.x, position.y);
  }

}
