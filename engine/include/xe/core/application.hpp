//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_APPLICATION_HPP
#define XE_APPLICATION_HPP

#include <xe/core/object.hpp>
#include <xe/core/event.hpp>
#include <xe/core/layer_base.hpp>

namespace xe {

  class LayerStack;

  class XE_API Application : public Object {
  XE_OBJECT(Application, Object);
    friend class Engine;
  public:
    explicit Application();
    ~Application() override;

  public:
    std::shared_ptr<LayerBase> currentLayer();

    void pushLayer(std::shared_ptr<LayerBase> layer);
    std::shared_ptr<LayerBase> popLayer(bool stop = true);
    void pushOverlay(std::shared_ptr<LayerBase> overlay);
    std::shared_ptr<LayerBase> popOverlay(bool stop = true);

    std::vector<std::shared_ptr<LayerBase>>::iterator begin();
    std::vector<std::shared_ptr<LayerBase>>::iterator end();
    std::vector<std::shared_ptr<LayerBase>>::reverse_iterator rbegin();
    std::vector<std::shared_ptr<LayerBase>>::reverse_iterator rend();

  protected:
    virtual void onInit() { }
    virtual void onStart();
    virtual void onStop();

    virtual void onPreUpdate();
    virtual void onUpdate();
    virtual void onPostUpdate();

    virtual void onPreRender();
    virtual void onRender();
    virtual void onPostRender();

    virtual void onKeyPressed(Event::Key e);
    virtual void onKeyReleased(Event::Key e);
    virtual void onKeyRepeated(Event::Key e);
    virtual void onTextEntered(Event::Text e);

    virtual void onMousePressed(Event::MouseButton e);
    virtual void onMouseReleased(Event::MouseButton e);
    virtual void onMouseScrolled(Event::MouseWheel e);
    virtual void onMouseMoved(Event::MouseMove e);

    virtual void onResize(Event::SizeEvent e);

    virtual void onMouseEntered();
    virtual void onMouseLeft();

    virtual void onFocusGained();
    virtual void onFocusLost();

  private:
    void onEvent(const Event &e);

  private:
    LayerStack *layerStack_;
  };

}

#endif //XE_APPLICATION_HPP
