//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/gpu.hpp>
#include <xe/graphics/window.hpp>
#include <xe/graphics/render_context.hpp>

namespace xe {

  GPU::GPU() :
      existing_(false),
      usedBuffers_(0),
      usedTextures_(0),
      usedMaterials_(0),
      usedFramebuffers_(0) {

    setName("GPU");
    window_ = make_ref<Window>();

    ctx_ = new RenderContext();

    logicFrame_ = make_scoped<DrawList>();
  }

  GPU::~GPU() {
    delete ctx_;
  }

  void GPU::init() {
    ctx_->init(params_);

    threadSync_.thread = std::thread(&xe::GPU::run, this);
    XE_CORE_INFO("[GPU] Launched rendering thread");
    std::unique_lock<std::mutex> lock(threadSync_.mxL);
    threadSync_.cvL.wait(lock, [this] { return threadSync_.initialized; });
  }

  void GPU::run() {
    XE_TRACE_META_THREAD_NAME("Render thread");
    window_->init();

    threadSync_.initialized = true;
    threadSync_.cvL.notify_one();

    XE_TRACE_BEGIN("XE", "Frame");
    XE_TRACE_BEGIN("XE", "Waiting (render)");

    while (!(existing_ = window_->isExisting())) {
      XE_CORE_TRACE("[GPU] GPU Synchronization (render waiting)");
      std::unique_lock<std::mutex> lock(threadSync_.mxR);
      threadSync_.cvR.wait(lock, [this] { return renderFrame_.commands_.empty(); });

      if (!threadSync_.nextFrame.commands_.empty()) {
        XE_TRACE_END("XE", "Waiting (render)");
        XE_CORE_TRACE("[GPU] GPU Synchronization (render start)");
        window_->pollEvents();
        renderFrame_.commands_.swap(threadSync_.nextFrame.commands_);

        threadSync_.cvL.notify_one();
        XE_CORE_TRACE("[GPU] GPU Synchronization (render ready)");

        renderFrame_.update();
        window_->swap();

        XE_TRACE_END("XE", "Frame");
        XE_TRACE_BEGIN("XE", "Frame");
        XE_TRACE_BEGIN("XE", "Waiting (render)");
      } else {
        threadSync_.cvL.notify_one();
        XE_CORE_TRACE("[GPU] GPU Synchronization (render ready)");
      }
    }

    XE_TRACE_END("XE", "Frame");
    XE_TRACE_END("XE", "Waiting (render)");

    window_->stop();
    threadSync_.cvL.notify_one();
  }

  void GPU::prepareRender() {
    XE_TRACE_BEGIN("XE", "Waiting (update)");
    XE_CORE_TRACE("[GPU] GPU Synchronization (update waiting)");
    std::unique_lock<std::mutex> lock(threadSync_.mxL);
    threadSync_.cvL.wait(lock, [this] { return existing_ || threadSync_.nextFrame.commands_.empty(); });
    XE_CORE_TRACE("[GPU] GPU Synchronization (update start)");
    XE_TRACE_END("XE", "Waiting (update)");
  }

  void GPU::execute() {
    if (logicFrame_) {
      threadSync_.nextFrame.commands_ = std::move(logicFrame_->commands_);
    }
    threadSync_.cvR.notify_one();
    XE_CORE_TRACE("[GPU] GPU Synchronization (logic ready)");
  }

  void GPU::appendCommands(DrawList &&dl) {
    if (logicFrame_->commands_.empty()) {
      logicFrame_->commands_ = std::move(dl.commands_);
    } else {
      logicFrame_->commands_.reserve(logicFrame_->commands_.size() + dl.commands_.size());
      std::move(std::begin(dl.commands_), std::end(dl.commands_), std::back_inserter(logicFrame_->commands_));
      dl.commands_.clear();
    }
  }

  void GPU::stop() {
    threadSync_.thread.join();
    XE_CORE_INFO("[GPU] Joined rendering thread");
  }

  template<class T>
  static uint acquireResource(scoped_array<T> *pool) {
    uint tryCount = 10;
    while (tryCount--) {
      for (uint i = 0; i < pool->size(); ++i) {
        if ((*pool)[i].acquire()) {
          const uint version = (*pool)[i].version;
          const uint result = i | (version << 20);
          return result;
        }
      }
    }
    XE_CORE_ERROR("[GPU] Assigned id 0 to resource instance");
    return 0;
  }

  gpu::Buffer GPU::createBuffer(const gpu::Buffer::Info &info) {
    const uint id = acquireResource(&ctx_->buffers_);
    const uint loc = RenderContext::index(id);
    gpu::BufferInstance &inst = ctx_->buffers_[loc];
    inst.info = info;

    ++usedBuffers_;

    return gpu::Buffer(ctx_, id);
  }

}
