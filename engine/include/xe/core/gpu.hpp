//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GPU_HPP
#define XE_GPU_HPP

#include <atomic>
#include <thread>
#include <mutex>
#include <memory>
#include <xe/core/object.hpp>
#include <xe/graphics/display_list.hpp>

namespace xe {

  class Window;

  class XE_API GPU : public Object {
  XE_OBJECT(GPU, Object);
    friend class Engine;
  public:
    ~GPU() override;

    inline void setParams(const Params::GPU &params) { params_ = params; }

    gpu::Buffer createBuffer(const gpu::Buffer::Info &info) const;
    gpu::Texture createTexture(const gpu::Texture::Info &info) const;
    gpu::Pipeline createPipeline(const gpu::Pipeline::Info &info) const;
    gpu::Framebuffer createFramebuffer(const gpu::Framebuffer::Info &info) const;

    inline uint32_t maxBuffers() const { return params_.maxBuffers; }
    inline uint32_t maxTextures() const { return params_.maxTextures; }
    inline uint32_t maxPipelines() const { return params_.maxPipelines; }
    inline uint32_t maxFramebuffers() const { return params_.maxFramebuffers; }

    inline uint32_t usedBuffers() const { return usedBuffers_; }
    inline uint32_t usedTextures() const { return usedTextures_; }
    inline uint32_t usedPipelines() const { return usedPipelines_; }
    inline uint32_t usedFramebuffers() const { return usedFramebuffers_; }

  protected:
    GPU();

    void init();
    void run();
    void prepareRender();
    void execute();
    void stop();

    void submitCommands(DisplayList &&dl);

    inline bool isExisting() const { return existing_; }

  protected:
    bool existing_;
    Params::GPU params_{ };

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
    } threadSync_;

  private:
    RenderContext *ctx_;

    mutable uint32_t usedBuffers_;
    mutable uint32_t usedTextures_;
    mutable uint32_t usedPipelines_;
    mutable uint32_t usedFramebuffers_;
  };

}

#endif //XE_GPU_HPP
