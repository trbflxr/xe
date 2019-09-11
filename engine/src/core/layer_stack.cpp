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
      l->onStop();
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

  void LayerStack::processEvents(Event &e) {

  }

  void LayerStack::pushLayer(const ref_ptr<Layer> &layer) {
    layer->onInit();
    layers_.emplace(layers_.begin() + index_, layer);
    ++index_;
  }

  ref_ptr<Layer> LayerStack::popLayer() {
    --index_;
    ref_ptr<Layer> layer = layers_[index_];
    layers_.erase(layers_.begin() + index_);
    layer->onStop();
    return layer;
  }

  void LayerStack::pushOverlay(const ref_ptr<Layer> &overlay) {
    overlay->onInit();
    layers_.push_back(overlay);
  }

  ref_ptr<Layer> LayerStack::popOverlay() {
    ref_ptr<Layer> overlay = layers_.back();
    layers_.pop_back();
    overlay->onStop();
    return overlay;
  }

}
