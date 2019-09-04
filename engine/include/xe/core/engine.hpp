//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_ENGINE_HPP
#define XE_ENGINE_HPP

#include <xe/memory.hpp>
#include <xe/core/gpu.hpp>
#include <xe/core/timestep.hpp>
#include <xe/graphics/window.hpp>

namespace xe {

  class XE_API Engine : public Object {
  XE_OBJECT(Engine, Object);
    friend class Application;
  public:
    ~Engine() override;

    static Engine &ref();

    void setParams(const Params &params);

    bool isExisting() const;

    void submitDrawList(DrawList &&dl);

    inline Window &window() { return *gpu_->window_; }
    inline GPU &gpu() { return *gpu_; }

  private:
    Engine();

    bool init();
    void start();
    void preUpdate();
    void update(Timestep ts);
    void postUpdate();
    void renderPreUpdate();
    void renderUpdate();
    void renderPostUpdate();
    void stop();

    void startSystems();
    void stopSystems();

  private:
    Params params_;

    ref_ptr<GPU> gpu_;

  };

}

#endif //XE_ENGINE_HPP
