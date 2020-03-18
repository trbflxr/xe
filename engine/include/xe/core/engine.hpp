//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_ENGINE_HPP
#define XE_ENGINE_HPP

#include <memory>
#include <queue>
#include <entt/entt.hpp>
#include <xe/params.hpp>
#include <xe/core/vfs.hpp>
#include <xe/core/gpu.hpp>
#include <xe/core/assets.hpp>
#include <xe/core/timestep.hpp>
#include <xe/core/application.hpp>
#include <xe/graphics/window.hpp>
#include <xe/graphics/composer.hpp>

namespace xe {

  class XE_API Engine : public Object {
  XE_OBJECT(Engine, Object);
  public:
    explicit Engine(int32_t argc, char **argv);
    ~Engine() override = default;

    static Engine &ref() { return *instance_; }

    void init(const Params &params);

    int32_t run();
    void requestClose();

    void setApp(std::shared_ptr<Application> &&app);

    bool isRunning() const;

    void executeOnGpu(DisplayList &&dl);

    Params getParams() const;

    void setUiFunction(const std::function<void(void *)> &function, void *data);

    void setTimestep(float step) { framerate_.timeStep = step; }
    void setMaxSteps(uint32_t steps) { framerate_.maxSteps = steps; }

    Timestep delta() const { return framerate_.timeStep; }
    uint32_t fps() const { return framerate_.fps; }
    uint32_t ups() const { return framerate_.ups; }

    std::shared_ptr<Application> &app() { return app_; }
    entt::registry &registry() { return *registry_; }
    VFS &vfs() { return *vfs_; }
    GPU &gpu() { return *gpu_; }
    Window &window() { return *gpu_->window_; }
    AssetManager &assetManager() { return *assetManager_; }
    Composer &composer() { return *composer_; }

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

    std::unique_ptr<entt::registry> registry_;

    std::vector<std::string> args_;

    std::shared_ptr<Application> app_;

    std::unique_ptr<VFS> vfs_;
    std::unique_ptr<GPU> gpu_;
    std::unique_ptr<AssetManager> assetManager_;

    std::unique_ptr<Composer> composer_;

    std::mutex eventsMutex_;
    std::queue<Event> events_;

    struct Framerate {
      float timeStep = 1.0f / 60.0f;
      uint32_t maxSteps = 10;
      uint32_t fps = 0;
      uint32_t ups = 0;
    } framerate_;
  };

}

#endif //XE_ENGINE_HPP
