//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_LAYER_STACK_HPP
#define XE_LAYER_STACK_HPP

#include <xe/common.hpp>
#include <xe/core/event.hpp>
#include <xe/core/timestep.hpp>
#include <xe/utils/noncopyable.hpp>

namespace xe {

  class Layer;

  class XE_API LayerStack : NonCopyable {
  public:
    explicit LayerStack();

    void stop();

    void render();

    virtual void preUpdate();
    virtual void update(Timestep ts);
    virtual void postUpdate();

    void processEvents(const Event &e);

    void pushLayer(const ref_ptr<Layer> &layer);
    ref_ptr<Layer> popLayer();
    void pushOverlay(const ref_ptr<Layer> &overlay);
    ref_ptr<Layer> popOverlay();

    inline ref_ptr<Layer> &topLayer() { return *(layers_.begin() + (index_ - 1)); }

    inline vector<ref_ptr<Layer>>::iterator begin() { return layers_.begin(); }
    inline vector<ref_ptr<Layer>>::iterator end() { return layers_.end(); }

  private:
    vector<ref_ptr<Layer>> layers_;
    size_t index_;
  };

}

#endif //XE_LAYER_STACK_HPP
