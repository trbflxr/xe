//
// Created by trbflxr on 6/13/2020.
//

#include "xepch.hpp"
#include "layer_stack.hpp"

namespace xe {

  LayerStack::LayerStack() :
      index_(0) { }

  void LayerStack::pushLayer(std::shared_ptr<LayerBase> layer) {
    layer->onInit();
    layers_.emplace(layers_.begin() + index_, std::move(layer));
    ++index_;
  }

  std::shared_ptr<LayerBase> LayerStack::popLayer(bool stop) {
    --index_;
    auto &&layer = layers_[index_];
    layers_.erase(layers_.begin() + index_);
    if (stop) {
      layer->onStop();
    }
    return layer;
  }

  void LayerStack::pushOverlay(std::shared_ptr<LayerBase> overlay) {
    overlay->onInit();
    layers_.push_back(std::move(overlay));
  }

  std::shared_ptr<LayerBase> LayerStack::popOverlay(bool stop) {
    auto &&layer = layers_.back();
    layers_.pop_back();
    if (stop) {
      layer->onStop();
    }
    return layer;
  }

  void LayerStack::onStart() {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      (*l)->onStart();
    }
  }

  void LayerStack::onStop() {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      (*l)->onStop();
    }
  }

  void LayerStack::onPreUpdate() {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onPreUpdate();
      }
    }
  }

  void LayerStack::onUpdate() {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onUpdate();
      }
    }
  }

  void LayerStack::onPostUpdate() {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onPostUpdate();
      }
    }
  }

  void LayerStack::onPreRender() {
    for (auto &&l : layers_) {
      if (l->isVisible()) {
        l->onPreRender();
      }
    }
  }

  void LayerStack::onRender() {
    for (auto &&l : layers_) {
      if (l->isVisible()) {
        l->onRender();
      }
    }
  }

  void LayerStack::onPostRender() {
    for (auto &&l : layers_) {
      if (l->isVisible()) {
        l->onPostRender();
      }
    }
  }

  void LayerStack::onKeyPressed(Event::Key e) {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onKeyPressed(e);
      }
    }
  }

  void LayerStack::onKeyReleased(Event::Key e) {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onKeyReleased(e);
      }
    }
  }

  void LayerStack::onKeyRepeated(Event::Key e) {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onKeyRepeated(e);
      }
    }
  }

  void LayerStack::onTextEntered(Event::Text e) {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onTextEntered(e);
      }
    }
  }

  void LayerStack::onMousePressed(Event::MouseButton e) {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onMousePressed(e);
      }
    }
  }

  void LayerStack::onMouseReleased(Event::MouseButton e) {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onMouseReleased(e);
      }
    }
  }

  void LayerStack::onMouseScrolled(Event::MouseWheel e) {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onMouseScrolled(e);
      }
    }
  }

  void LayerStack::onMouseMoved(Event::MouseMove e) {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onMouseMoved(e);
      }
    }
  }

  void LayerStack::onResize(Event::SizeEvent e) {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onResize(e);
      }
    }
  }

  void LayerStack::onMouseEntered() {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onMouseEntered();
      }
    }
  }

  void LayerStack::onMouseLeft() {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onMouseLeft();
      }
    }
  }

  void LayerStack::onFocusGained() {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onFocusGained();
      }
    }
  }

  void LayerStack::onFocusLost() {
    for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
      if ((*l)->isActive()) {
        (*l)->onFocusLost();
      }
    }
  }

  void LayerStack::onUi() {
    for (auto &&l : layers_) {
      if (l->isVisible()) {
        l->onUi();
      }
    }
  }

}
