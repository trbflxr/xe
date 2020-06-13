//
// Created by trbflxr on 6/13/2020.
//

#ifndef XE_LAYER_STACK_HPP
#define XE_LAYER_STACK_HPP

#include <memory>
#include <xe/core/object.hpp>
#include <xe/core/event.hpp>
#include <xe/core/layer_base.hpp>

namespace xe {

  class XE_API LayerStack : public Object {
  XE_OBJECT(LayerStack, Object);
  public:
    explicit LayerStack();

    ~LayerStack() override = default;

    void pushLayer(std::shared_ptr<LayerBase> layer);
    std::shared_ptr<LayerBase> popLayer(bool stop = true);
    void pushOverlay(std::shared_ptr<LayerBase> overlay);
    std::shared_ptr<LayerBase> popOverlay(bool stop = true);

    std::shared_ptr<LayerBase> currentLayer() { return *(layers_.begin() + (index_ - 1)); }

    std::vector<std::shared_ptr<LayerBase>>::iterator begin() { return layers_.begin(); }
    std::vector<std::shared_ptr<LayerBase>>::iterator end() { return layers_.end(); }
    std::vector<std::shared_ptr<LayerBase>>::reverse_iterator rbegin() { return layers_.rbegin(); }
    std::vector<std::shared_ptr<LayerBase>>::reverse_iterator rend() { return layers_.rend(); }

    void onStart();
    void onStop();

    void onPreUpdate();
    void onUpdate();
    void onPostUpdate();

    void onPreRender();
    void onRender();
    void onPostRender();

    void onKeyPressed(Event::Key e);
    void onKeyReleased(Event::Key e);
    void onKeyRepeated(Event::Key e);
    void onTextEntered(Event::Text e);

    void onMousePressed(Event::MouseButton e);
    void onMouseReleased(Event::MouseButton e);
    void onMouseScrolled(Event::MouseWheel e);
    void onMouseMoved(Event::MouseMove e);

    void onResize(Event::SizeEvent e);

    void onMouseEntered();
    void onMouseLeft();

    void onFocusGained();
    void onFocusLost();

    void onUi();

  private:
    std::vector<std::shared_ptr<LayerBase>> layers_;
    size_t index_;
  };

}

#endif //XE_LAYER_STACK_HPP
