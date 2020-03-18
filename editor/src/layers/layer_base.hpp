//
// Created by trbflxr on 3/18/2020.
//

#ifndef XE_LAYER_BASE_HPP
#define XE_LAYER_BASE_HPP

#include <xe/core/event.hpp>

namespace xe {

  class LayerBase {
  public:
    virtual void onInit() { }
    virtual void onStart() { }
    virtual void onPreUpdate() { }
    virtual void onUpdate() { }
    virtual void onPostUpdate() { }
    virtual void onPreRender() { }
    virtual void onRender() { }
    virtual void onPostRender() { }
    virtual void onStop() { }

    virtual bool onKeyPressed(const Event::Key /*e*/) { return false; }
    virtual bool onKeyReleased(const Event::Key /*e*/) { return false; }
    virtual bool onKeyRepeated(const Event::Key /*e*/) { return false; }
    virtual bool onTextEntered(const Event::Text /*e*/) { return false; }

    virtual bool onMousePressed(const Event::MouseButton /*e*/) { return false; }
    virtual bool onMouseReleased(const Event::MouseButton /*e*/) { return false; }
    virtual bool onMouseScrolled(const Event::MouseWheel /*e*/) { return false; }
    virtual bool onMouseMoved(const Event::MouseMove /*e*/) { return false; }

    virtual bool onResize(const Event::SizeEvent /*e*/) { return false; }

    virtual bool onMouseEntered() { return false; }
    virtual bool onMouseLeft() { return false; }

    virtual bool onFocusGained() { return false; }
    virtual bool onFocusLost() { return false; }

    virtual bool onUi() { return false; }
  };

}

#endif //XE_LAYER_BASE_HPP
