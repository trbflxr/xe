//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "gl_window.hpp"
#include <xe/utils/logger.hpp>

namespace xe {

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

    glfwSetWindowUserPointer(data->window, data);

    glfwSetCursorPosCallback(data->window, [](GLFWwindow *window, double xPos, double yPos) {
      Window::Data &data = *(Window::Data *) glfwGetWindowUserPointer(window);

      Event e;
      e.type = Event::MouseMoved;
      e.mouseMove.x = static_cast<float>(xPos);
      e.mouseMove.y = static_cast<float>(yPos);
      data.events.push(e);
    });

//    glfwSetKeyCallback(data->window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
//      XE_CORE_CRITICAL("[Event] Key (action: {}, {}, {})", action, key, mods);
//      GLFWwindow *w = (GLFWwindow *) glfwGetWindowUserPointer(window);
//
//      string title((uint) key);
//      glfwSetWindowTitle(w, title.c_str());
//    });
  }

  void WindowBackend::initContext(Window::Data *data) {
    glfwMakeContextCurrent(data->window);

    const int32 status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if (!status) {
      XE_CORE_CRITICAL("[WindowBackend / GL] Failed to initialize Glad");
      return;
    }

    setIcon(data);
    setSwapInterval(data);


    const byte *version = glGetString(GL_VERSION);
    const byte *vendor = glGetString(GL_VENDOR);
    const byte *renderer = glGetString(GL_RENDERER);

    XE_CORE_INFO("[WindowBackend / GL] Window initialized successful:\n"
                 "\t- glVersion \t\t({})\n"
                 "\t- glVendor \t\t({})\n"
                 "\t- glRenderer \t\t({})",
                 version, vendor, renderer);
  }

  void WindowBackend::swap(Window::Data *data) {
    glfwSwapBuffers(data->window);
  }

  void WindowBackend::pollEvents(Window::Data *data) {
    glfwPollEvents();
  }

  void WindowBackend::stop(Window::Data *data) {
    glfwTerminate();
  }

  double WindowBackend::uptime(Window::Data *data) {
    return glfwGetTime();
  }

  bool WindowBackend::isExisting(Window::Data *data) {
    return glfwWindowShouldClose(data->window);
  }

  void WindowBackend::forceExit(Window::Data *data) {
    glfwSetWindowShouldClose(data->window, GLFW_TRUE);
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

  bool WindowBackend::isKeyPressed(Window::Data *data, Keyboard::Key key) {
    int32 glfwCode = 0;
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
      case Keyboard::PageDown:glfwCode = GLFW_KEY_PAGE_DOWN;
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

    const int32 state = glfwGetKey(data->window, glfwCode);
    return state == GLFW_PRESS;
  }

  bool WindowBackend::isMouseButtonPressed(Window::Data *data, Mouse::Button button) {
    int32 glfwButton = 0;
    switch (button) {
      case Mouse::Left: glfwButton = GLFW_MOUSE_BUTTON_1;
        break;
      case Mouse::Right: glfwButton = GLFW_MOUSE_BUTTON_2;
        break;
      case Mouse::Middle: glfwButton = GLFW_MOUSE_BUTTON_3;
        break;
      case Mouse::XButton1: glfwButton = GLFW_MOUSE_BUTTON_4;
        break;
      case Mouse::XButton2: glfwButton = GLFW_MOUSE_BUTTON_5;
        break;
      case Mouse::XButton3: glfwButton = GLFW_MOUSE_BUTTON_6;
        break;
      case Mouse::XButton4: glfwButton = GLFW_MOUSE_BUTTON_7;
        break;
      case Mouse::XButton5: glfwButton = GLFW_MOUSE_BUTTON_8;
        break;
      default: break;
    }

    const int32 state = glfwGetMouseButton(data->window, glfwButton);
    return state == GLFW_PRESS;
  }

  vec2 WindowBackend::getMousePosition(Window::Data *data) {
    double x;
    double y;
    glfwGetCursorPos(data->window, &x, &y);
    return vec2(static_cast<float>(x), static_cast<float>(y));
  }

  void WindowBackend::setMousePosition(Window::Data *data, const vec2 &position) {
    glfwSetCursorPos(data->window, position.x, position.y);
  }

}
