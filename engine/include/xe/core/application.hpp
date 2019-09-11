//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_APPLICATION_HPP
#define XE_APPLICATION_HPP

#include <xe/core/engine.hpp>
#include <xe/core/layer_stack.hpp>

namespace xe {

  class XE_API Application : public Object {
  XE_OBJECT(Application, Object);
  public:
    explicit Application();
    ~Application() override;

    ref_ptr<Layer> &topLayer();

    void pushLayer(const ref_ptr<Layer> &layer);
    ref_ptr<Layer> popLayer();
    void pushOverlay(const ref_ptr<Layer> &overlay);
    ref_ptr<Layer> popOverlay();

    bool isExisting() const;

    inline void setTimestep(float step) { framerate_.timeStep = step; }
    inline void setMaxSteps(uint steps) { framerate_.maxSteps = steps; }

    inline Timestep delta() const { return framerate_.timeStep; }
    inline uint fps() const { return framerate_.fps; }

    int32 run(int32 argc, char **argv);

  protected:
    virtual void init() { }
    virtual void start() { }
    virtual void preUpdate() { }
    virtual void update(Timestep ts) { }
    virtual void postUpdate() { }
    virtual void preRender() { }
    virtual void render() { }
    virtual void postRender() { }
    virtual void stop() { }

  private:
    void initInternal();
    void startInternal();
    void preUpdateInternal();
    void updateInternal(Timestep ts);
    void postUpdateInternal();
    void preRenderInternal();
    void renderInternal();
    void postRenderInternal();
    void stopInternal();

    void processEvents();

  private:
    LayerStack layerStack_;

    bool exit_;

    struct Framerate {
      float timeStep;
      uint maxSteps;
      uint fps;
    } framerate_;
  };

}

#endif //XE_APPLICATION_HPP
