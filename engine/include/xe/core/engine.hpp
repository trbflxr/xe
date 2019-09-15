//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_ENGINE_HPP
#define XE_ENGINE_HPP

#include <xe/memory.hpp>
#include <xe/core/gpu.hpp>
#include <xe/core/assets.hpp>
#include <xe/core/timestep.hpp>
#include <xe/core/scene.hpp>
#include <xe/graphics/window.hpp>
#include <xe/components/transform.hpp>

namespace xe {

  class XE_API Engine : public Object {
  XE_OBJECT(Engine, Object);
    friend class Application;
  public:
    ~Engine() override;

    static Engine &ref();

    void setParams(const Params &params);

    bool isExisting() const;

    void submitDrawList(DisplayList &&dl);

    void setUiFunction(const std::function<void(void *)> &function, void *data);

    void loadScene(const ref_ptr<Scene> &scene);
    inline const ref_ptr<Scene> &scene() { return scene_; }

    inline Window &window() { return *gpu_->window_; }
    inline GPU &gpu() { return *gpu_; }
    inline AssetManager &assetManager() { return *assetManager_; }

    inline const ref_ptr<System::Transform> &transform() { return transform_; }

    static bool isKeyPressed(Keyboard::Key key);
    static bool isMouseButtonPressed(Mouse::Button button);

    static vec2 getMousePosition();
    static void setMousePosition(const vec2 &position);

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

    ref_ptr<Scene> scene_;
    ref_ptr<GPU> gpu_;
    ref_ptr<AssetManager> assetManager_;

    ref_ptr<System::Transform> transform_;
  };

}

#endif //XE_ENGINE_HPP
