//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_GPU_HPP
#define XE_GPU_HPP

#include <atomic>
#include <thread>
#include <mutex>
#include <xe/memory.hpp>
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

    inline uint maxBuffers() const { return params_.maxBuffers; }
    inline uint maxTextures() const { return params_.maxTextures; }
    inline uint maxPipelines() const { return params_.maxPipelines; }
    inline uint maxFramebuffers() const { return params_.maxFramebuffers; }

    inline uint usedBuffers() const { return usedBuffers_; }
    inline uint usedTextures() const { return usedTextures_; }
    inline uint usedPipelines() const { return usedPipelines_; }
    inline uint usedFramebuffers() const { return usedFramebuffers_; }

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

    ref_ptr<Window> window_;

    scoped_ptr<DisplayList> logicFrame_;
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

    mutable uint usedBuffers_;
    mutable uint usedTextures_;
    mutable uint usedPipelines_;
    mutable uint usedFramebuffers_;
  };

}

#endif //XE_GPU_HPP
