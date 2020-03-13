//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_ENGINE_HPP
#define XE_ENGINE_HPP

#include <memory>
#include <queue>
#include <xe/params.hpp>
#include <xe/core/vfs.hpp>
#include <xe/core/gpu.hpp>
#include <xe/core/assets.hpp>
#include <xe/core/timestep.hpp>
#include <xe/core/application.hpp>
#include <xe/core/scene.hpp>
#include <xe/graphics/window.hpp>
#include <xe/components/transform.hpp>

namespace xe {

  class XE_API Engine : public Object {
  XE_OBJECT(Engine, Object);
  public:
    explicit Engine(int32_t argc, char **argv);
    ~Engine() override;

    static Engine &ref() { return *instance_; }

    int32_t run();
    void requestClose();

    Application *getApp() const { return app_.get(); }
    void setApp(std::unique_ptr<Application> &&app) { app_ = std::move(app); }

    bool isRunning() const;

    void submitDrawList(DisplayList &&dl);

    void setParams(const Params &params);
    Params getParams() const;

    void setUiFunction(const std::function<void(void *)> &function, void *data);

    void loadScene(const std::shared_ptr<Scene> &scene);
    const std::shared_ptr<Scene> &scene() { return scene_; }

    void setTimestep(float step) { framerate_.timeStep = step; }
    void setMaxSteps(uint32_t steps) { framerate_.maxSteps = steps; }

    Timestep delta() const { return framerate_.timeStep; }
    uint32_t fps() const { return framerate_.fps; }

    VFS &vfs() { return *vfs_; }
    GPU &gpu() { return *gpu_; }
    Window &window() { return *gpu_->window_; }
    AssetManager &assetManager() { return *assetManager_; }

    const std::shared_ptr<System::Transform> &transform() { return transform_; }

    const std::vector<std::string> &getArgs() const { return args_; }

    void pushEvents(const std::vector<Event> &events);

    static bool isKeyPressed(Keyboard::Key key);
    static bool isMouseButtonPressed(Mouse::Button button);

    static vec2 getMousePosition();
    static void setMousePosition(const vec2 &position);

  private:
    bool init();
    void start();
    void preUpdate();
    void update(Timestep ts);
    void postUpdate();
    void renderPreUpdate();
    void renderUpdate();
    void renderPostUpdate();
    void stop();

    void processEvents();

    void startSystems();
    void stopSystems();

  private:
    static Engine *instance_;

    std::vector<std::string> args_;

    std::unique_ptr<Application> app_;
    std::shared_ptr<Scene> scene_;

    std::unique_ptr<VFS> vfs_;
    std::unique_ptr<GPU> gpu_;
    std::unique_ptr<AssetManager> assetManager_;

    std::shared_ptr<System::Transform> transform_;

    std::mutex eventsMutex_;
    std::queue<Event> events_;

    struct Framerate {
      float timeStep = 1.0f / 60.0f;
      uint32_t maxSteps = 10;
      uint32_t fps = 0;
    } framerate_;
  };

}

#endif //XE_ENGINE_HPP
