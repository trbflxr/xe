//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_ENGINE_HPP
#define XE_ENGINE_HPP

#include <memory>
#include <xe/core/vfs.hpp>
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
    explicit Engine(int32_t argc, char **argv);
    ~Engine() override;

    static Engine &ref() { return *instance_; }

    bool isExisting() const;

    void submitDrawList(DisplayList &&dl);

    void setParams(const Params &params);

    void setUiFunction(const std::function<void(void *)> &function, void *data);

    void loadScene(const std::shared_ptr<Scene> &scene);
    const std::shared_ptr<Scene> &scene() { return scene_; }

    VFS &vfs() { return *vfs_; }
    GPU &gpu() { return *gpu_; }
    Window &window() { return *gpu_->window_; }
    AssetManager &assetManager() { return *assetManager_; }

    const std::shared_ptr<System::Transform> &transform() { return transform_; }

    const std::vector<std::string> &getArgs() const { return args_; }

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

    void startSystems();
    void stopSystems();

  private:
    static Engine *instance_;

    Params params_;

    std::vector<std::string> args_;

    std::shared_ptr<Scene> scene_;

    std::unique_ptr<VFS> vfs_;
    std::unique_ptr<GPU> gpu_;
    std::unique_ptr<AssetManager> assetManager_;

    std::shared_ptr<System::Transform> transform_;
  };

}

#endif //XE_ENGINE_HPP
