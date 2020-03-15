//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_APPLICATION_HPP
#define XE_APPLICATION_HPP

#include <xe/core/object.hpp>
#include <xe/core/event.hpp>

namespace xe {

  class XE_API Application : public Object {
  XE_OBJECT(Application, Object);
    friend class Engine;
  public:
    explicit Application() {
      setName("Application");
    }

    ~Application() override = default;

  protected:
    virtual void onInit() { }
    virtual void onStart() { }
    virtual void onPreUpdate() { }
    virtual void onUpdate() { }
    virtual void onPostUpdate() { }
    virtual void onPreRender() { }
    virtual void onRender() { }
    virtual void onPostRender() { }
    virtual void onStop() { }

    virtual void onKeyPressed(const Event::Key &/*e*/) { }
    virtual void onKeyReleased(const Event::Key &/*e*/) { }
    virtual void onKeyRepeated(const Event::Key &/*e*/) { }
    virtual void onTextEntered(const Event::Text &/*e*/) { }

    virtual void onMousePressed(const Event::MouseButton &/*e*/) { }
    virtual void onMouseReleased(const Event::MouseButton &/*e*/) { }
    virtual void onMouseScrolled(const Event::MouseWheel &/*e*/) { }
    virtual void onMouseMoved(const Event::MouseMove &/*e*/) { }

    virtual void onResize(const Event::SizeEvent &/*e*/) { }

    virtual void onMouseEntered() { }
    virtual void onMouseLeft() { }

    virtual void onFocusGained() { }
    virtual void onFocusLost() { }

  private:
    void onEvent(const Event &e) {
      switch (e.type) {
        case Event::Closed: {
          onStop();
          break;
        }
        case Event::Resized: {
          onResize(e.size);
          break;
        }
        case Event::LostFocus: {
          onFocusLost();
          break;
        }
        case Event::GainedFocus: {
          onFocusGained();
          break;
        }
        case Event::TextEntered: {
          onTextEntered(e.text);
          break;
        }
        case Event::KeyPressed: {
          onKeyPressed(e.key);
          break;
        }
        case Event::KeyRepeated: {
          onKeyRepeated(e.key);
          break;
        }
        case Event::KeyReleased: {
          onKeyReleased(e.key);
          break;
        }
        case Event::MouseScrolled: {
          onMouseScrolled(e.mouseScroll);
          break;
        }
        case Event::MouseButtonPressed: {
          onMousePressed(e.mouseButton);
          break;
        }
        case Event::MouseButtonReleased: {
          onMouseReleased(e.mouseButton);
          break;
        }
        case Event::MouseMoved: {
          onMouseMoved(e.mouseMove);
          break;
        }
        case Event::MouseEntered: {
          onMouseEntered();
          break;
        }
        case Event::MouseLeft: {
          onMouseLeft();
          break;
        }
        default: break;
      }
    }
  };

}

#endif //XE_APPLICATION_HPP
