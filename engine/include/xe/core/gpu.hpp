//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GPU_HPP
#define XE_GPU_HPP

#include <atomic>
#include <thread>
#include <mutex>
#include <memory>
#include <xe/params.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/display_list.hpp>

namespace xe {

  class Window;

  namespace gpu {
    class Backend;
  }

  class XE_API GPU : public Object {
  XE_OBJECT(GPU, Object);
    friend class Engine;
    friend struct WindowBackend;
    friend class gpu::Backend;
  public:
    enum class Vendor {
      AMD,
      Nvidia,
      Intel,
      Invalid
    };

  public:
    ~GPU() override;

    std::shared_ptr<gpu::Buffer> createBuffer(const gpu::Buffer::Info &info) const;
    std::shared_ptr<gpu::Texture> createTexture(const gpu::Texture::Info &info) const;
    std::shared_ptr<gpu::Pipeline> createPipeline(const gpu::Pipeline::Info &info) const;
    std::shared_ptr<gpu::Framebuffer> createFramebuffer(const gpu::Framebuffer::Info &info) const;

    void destroyResource(const gpu::Resource &resource) const;

    uint32_t maxBuffers() const { return params_.maxBuffers; }
    uint32_t maxTextures() const { return params_.maxTextures; }
    uint32_t maxPipelines() const { return params_.maxPipelines; }
    uint32_t maxFramebuffers() const { return params_.maxFramebuffers; }

    uint32_t usedBuffers() const { return usedBuffers_; }
    uint32_t usedTextures() const { return usedTextures_; }
    uint32_t usedPipelines() const { return usedPipelines_; }
    uint32_t usedFramebuffers() const { return usedFramebuffers_; }

    Vendor vendor() const { return vendor_; }

    static uint32_t drawCalls();
    static uint32_t gpuCommands();

  private:
    GPU();

    void init();
    void run();
    void prepareRender();
    void execute();
    void stop();

    void submitCommands(DisplayList &&dl);

    bool shouldStop() const { return shouldStop_; }

  private:
    bool shouldStop_ = false;
    Params::GPU params_{ };
    Vendor vendor_ = Vendor::Invalid;

    std::shared_ptr<Window> window_;

    std::unique_ptr<DisplayList> logicFrame_;
    DisplayList renderFrame_;

    struct ThreadSync {
      DisplayList nextFrame;
      std::thread thread;
      std::mutex mxR;
      std::mutex mxL;
      std::condition_variable cvR;
      std::condition_variable cvL;
      bool initialized;
      bool exit;
    } threadSync_;

    RenderContext *ctx_ = nullptr;

    mutable uint32_t usedBuffers_ = 0;
    mutable uint32_t usedTextures_ = 0;
    mutable uint32_t usedPipelines_ = 0;
    mutable uint32_t usedFramebuffers_ = 0;

    static std::atomic<uint32_t> gpuCommands_;
  };

}

#endif //XE_GPU_HPP
