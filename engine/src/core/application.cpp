//
// Created by trbflxr on 6/13/2020.
//

#include "xepch.hpp"
#include "xe/core/application.hpp"
#include "layer_stack.hpp"

namespace xe {

  Application::Application() {
    setName("Application");

    layerStack_ = new LayerStack();
  }

  Application::~Application() {
    delete layerStack_;
  }

  std::shared_ptr<LayerBase> Application::currentLayer() {
    return layerStack_->currentLayer();
  }

  void Application::pushLayer(std::shared_ptr<LayerBase> layer) {
    layerStack_->pushLayer(std::move(layer));
  }

  std::shared_ptr<LayerBase> Application::popLayer(bool stop) {
    return layerStack_->popLayer(stop);
  }

  void Application::pushOverlay(std::shared_ptr<LayerBase> overlay) {
    layerStack_->pushOverlay(std::move(overlay));
  }

  std::shared_ptr<LayerBase> Application::popOverlay(bool stop) {
    return layerStack_->popOverlay(stop);
  }

  std::vector<std::shared_ptr<LayerBase>>::iterator Application::begin() {
    return layerStack_->begin();
  }

  std::vector<std::shared_ptr<LayerBase>>::iterator Application::end() {
    return layerStack_->end();
  }

  std::vector<std::shared_ptr<LayerBase>>::reverse_iterator Application::rbegin() {
    return layerStack_->rbegin();
  }

  std::vector<std::shared_ptr<LayerBase>>::reverse_iterator Application::rend() {
    return layerStack_->rend();
  }

  void Application::onStart() {
    layerStack_->onStart();
  }

  void Application::onStop() {
    layerStack_->onStop();
  }

  void Application::onPreUpdate() {
    layerStack_->onPreUpdate();
  }

  void Application::onUpdate() {
    layerStack_->onUpdate();
  }

  void Application::onPostUpdate() {
    layerStack_->onPostUpdate();
  }

  void Application::onPreRender() {
    layerStack_->onPreRender();
  }

  void Application::onRender() {
    layerStack_->onRender();
  }

  void Application::onPostRender() {
    layerStack_->onPostRender();
  }

  void Application::onKeyPressed(Event::Key e) {
    layerStack_->onKeyPressed(e);
  }

  void Application::onKeyReleased(Event::Key e) {
    layerStack_->onKeyReleased(e);
  }

  void Application::onKeyRepeated(Event::Key e) {
    layerStack_->onKeyRepeated(e);
  }

  void Application::onTextEntered(Event::Text e) {
    layerStack_->onTextEntered(e);
  }

  void Application::onMousePressed(Event::MouseButton e) {
    layerStack_->onMousePressed(e);
  }

  void Application::onMouseReleased(Event::MouseButton e) {
    layerStack_->onMouseReleased(e);
  }

  void Application::onMouseScrolled(Event::MouseWheel e) {
    layerStack_->onMouseScrolled(e);
  }

  void Application::onMouseMoved(Event::MouseMove e) {
    layerStack_->onMouseMoved(e);
  }

  void Application::onResize(Event::SizeEvent e) {
    layerStack_->onResize(e);
  }

  void Application::onMouseEntered() {
    layerStack_->onMouseEntered();
  }

  void Application::onMouseLeft() {
    layerStack_->onMouseLeft();
  }

  void Application::onFocusGained() {
    layerStack_->onFocusGained();
  }

  void Application::onFocusLost() {
    layerStack_->onFocusLost();
  }

  void Application::onEvent(const Event &e) {
    switch (e.type) {
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

}
