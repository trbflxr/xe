//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_LAYER_HPP
#define XE_LAYER_HPP

#include <xe/core/application.hpp>

namespace xe {

  class Layer : public Object {
  XE_OBJECT(Layer, Object)
  public:
    explicit Layer(Application &app) :
        app_(app),
        window_(Engine::ref().window()),
        visible_(true),
        active_(true) { }

    virtual void onInit() { }
    virtual void onClose() { }

    virtual void onRender() { }

    virtual void onPreUpdate() { }
    virtual void onUpdate(Timestep ts) { }
    virtual void onPostUpdate() { }

    virtual bool onKeyPressed(const Event::Key &e) { return false; }
    virtual bool onKeyReleased(const Event::Key &e) { return false; }
    virtual bool onKeyRepeated(const Event::Key &e) { return false; }
    virtual bool onTextEntered(const Event::Text &e) { return false; }

    virtual bool onMousePressed(const Event::MouseButton &e) { return false; }
    virtual bool onMouseReleased(const Event::MouseButton &e) { return false; }
    virtual bool onMouseScrolled(const Event::MouseWheel &e) { return false; }
    virtual bool onMouseMoved(const Event::MouseMove &e) { return false; }

    virtual bool onResize(const Event::SizeEvent &e) { return false; }

    virtual void onMouseEntered() { }
    virtual void onMouseLeft() { }

    virtual void onFocusGained() { }
    virtual void onFocusLost() { }

    inline bool isVisible() const { return visible_; }
    inline void setVisible(bool visible) { visible_ = visible; }

    inline bool isActive() const { return active_; }
    inline void setActive(bool active) { active_ = active; }

  protected:
    Application &app_;
    Window &window_;
    bool visible_;
    bool active_;
  };

}

#endif //XE_LAYER_HPP
