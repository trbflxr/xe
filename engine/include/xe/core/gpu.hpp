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
#include <xe/graphics/draw_list.hpp>

namespace xe {

  class Window;

  class XE_API GPU : public Object {
  XE_OBJECT(GPU, Object);
    friend class Engine;
  public:
    GPU();
    ~GPU() override;

    inline void setParams(const Params::GPU &params) { params_ = params; }

    gpu::Buffer createBuffer(const gpu::Buffer::Info &info);

    inline uint maxBuffers() const { return params_.maxBuffers; }
    inline uint maxTextures() const { return params_.maxTextures; }
    inline uint maxMaterials() const { return params_.maxMaterials; }
    inline uint maxFramebuffers() const { return params_.maxFramebuffers; }

    inline uint usedBuffers() const { return usedBuffers_; }
    inline uint usedTextures() const { return usedTextures_; }
    inline uint usedMaterials() const { return usedMaterials_; }
    inline uint usedFramebuffers() const { return usedFramebuffers_; }

  protected:
    void init();
    void run();
    void prepareRender();
    void execute();
    void stop();

    void appendCommands(DrawList &&dl);

    inline bool isExisting() const { return existing_; }

  protected:
    bool existing_;
    Params::GPU params_{ };

    ref_ptr<Window> window_;

    scoped_ptr<DrawList> logicFrame_;
    DrawList renderFrame_;

    struct ThreadSync {
      DrawList nextFrame;
      std::thread thread;
      std::mutex mxR;
      std::mutex mxL;
      std::condition_variable cvR;
      std::condition_variable cvL;
      bool initialized;
    } threadSync_;

  private:
    RenderContext *ctx_;

    uint usedBuffers_;
    uint usedTextures_;
    uint usedMaterials_;
    uint usedFramebuffers_;
  };

}

#endif //XE_GPU_HPP
