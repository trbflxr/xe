//
// Created by FLXR on 9/11/2019.
//

#include "xepch.hpp"
#include <xe/core/layer_stack.hpp>
#include <xe/core/layer.hpp>

namespace xe {

  LayerStack::LayerStack() :
      index_(0) { }

  void LayerStack::stop() {
    for (auto &&l : layers_) {
      l->onClose();
    }
  }

  void LayerStack::render() {
    for (auto &&l : layers_) {
      l->onRender();
    }
  }

  void LayerStack::preUpdate() {
    for (auto &&l : layers_) {
      l->onPreUpdate();
    }
  }

  void LayerStack::update(Timestep ts) {
    for (auto &&l : layers_) {
      l->onUpdate(ts);
    }
  }

  void LayerStack::postUpdate() {
    for (auto &&l : layers_) {
      l->onPostUpdate();
    }
  }

  void LayerStack::processEvents(const Event &e) {
    bool handled = false;
    for (auto it = layers_.end(); it != layers_.begin();) {
      if ((*--it)->isActive()) {
        switch (e.type) {
          case Event::Closed: {
            (*it)->onClose();
            break;
          }
          case Event::Resized: {
            handled = (*it)->onResize(e.size);
            break;
          }
          case Event::LostFocus: {
            (*it)->onFocusLost();
            break;
          }
          case Event::GainedFocus: {
            (*it)->onFocusGained();
            break;
          }
          case Event::TextEntered: {
            handled = (*it)->onTextEntered(e.text);
            break;
          }
          case Event::KeyPressed: {
            handled = (*it)->onKeyPressed(e.key);
            break;
          }
          case Event::KeyRepeated: {
            handled = (*it)->onKeyRepeated(e.key);
            break;
          }
          case Event::KeyReleased: {
            handled = (*it)->onKeyReleased(e.key);
            break;
          }
          case Event::MouseScrolled: {
            handled = (*it)->onMouseScrolled(e.mouseScroll);
            break;
          }
          case Event::MouseButtonPressed: {
            handled = (*it)->onMousePressed(e.mouseButton);
            break;
          }
          case Event::MouseButtonReleased: {
            handled = (*it)->onMouseReleased(e.mouseButton);
            break;
          }
          case Event::MouseMoved: {
            handled = (*it)->onMouseMoved(e.mouseMove);
            break;
          }
          case Event::MouseEntered: {
            (*it)->onMouseEntered();
            break;
          }
          case Event::MouseLeft: {
            (*it)->onMouseLeft();
            break;
          }
          default: break;
        }
      }
      if (handled) return;
    }
  }

  void LayerStack::pushLayer(const std::shared_ptr<Layer> &layer) {
    layer->onInit();
    layers_.emplace(layers_.begin() + index_, layer);
    ++index_;
  }

  std::shared_ptr<Layer> LayerStack::popLayer() {
    --index_;
    std::shared_ptr<Layer> layer = layers_[index_];
    layers_.erase(layers_.begin() + index_);
    layer->onClose();
    return layer;
  }

  void LayerStack::pushOverlay(const std::shared_ptr<Layer> &overlay) {
    overlay->onInit();
    layers_.push_back(overlay);
  }

  std::shared_ptr<Layer> LayerStack::popOverlay() {
    std::shared_ptr<Layer> overlay = layers_.back();
    layers_.pop_back();
    overlay->onClose();
    return overlay;
  }

}
