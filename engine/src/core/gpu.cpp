//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/gpu.hpp>
#include <xe/graphics/window.hpp>
#include <xe/graphics/render_context.hpp>

namespace xe {

  template<class T>
  static uint acquireResource(memory<T> *pool) {
    uint tryCount = 10;
    while (tryCount--) {
      for (uint i = 0; i < pool->size; ++i) {
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

  GPU::GPU() :
      existing_(false),
      usedBuffers_(0),
      usedTextures_(0),
      usedPipelines_(0),
      usedFramebuffers_(0) {

    setName("GPU");
    window_ = make_ref<Window>();

    ctx_ = new RenderContext();

    logicFrame_ = make_scoped<DisplayList>();
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

  void GPU::submitCommands(DisplayList &&dl) {
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

  gpu::Buffer GPU::createBuffer(const gpu::Buffer::Info &info) {
    const uint id = acquireResource(&ctx_->buffers_);
    const uint loc = RenderContext::index(id);
    gpu::BufferInstance &inst = ctx_->buffers_[loc];
    inst.info = info;

    ++usedBuffers_;

    return gpu::Buffer(ctx_, id);
  }

  gpu::Texture GPU::createTexture(const gpu::Texture::Info &info) {
    if (info.format == TexelsFormat::None) {
      XE_CORE_ERROR("[GPU] Could not create texture: texels format not found");
      return gpu::Texture();
    }

    const uint id = acquireResource(&ctx_->textures_);
    const uint loc = RenderContext::index(id);
    gpu::TextureInstance &inst = ctx_->textures_[loc];
    inst.info = info;

    switch (info.format) {
      case TexelsFormat::R8: inst.bpp = 1;
        break;
      case TexelsFormat::Rg8: inst.bpp = 2;
        break;
      case TexelsFormat::Rgb8: inst.bpp = 3;
        break;
      case TexelsFormat::Rgba8: inst.bpp = 4;
        break;
      case TexelsFormat::Depth16: inst.bpp = 2;
        break;
      case TexelsFormat::DepthStencil16: inst.bpp = 4;
        break;
      default: break;
    }

    ++usedTextures_;

    return gpu::Texture{ctx_, id};
  }

  gpu::Pipeline GPU::createPipeline(const gpu::Pipeline::Info &info) {
    const uint id = acquireResource(&ctx_->pipelines_);
    const uint loc = RenderContext::index(id);
    gpu::PipelineInstance &inst = ctx_->pipelines_[loc];
    inst.info = info;

    inst.vertShader = info.shader.vert;
    inst.tessControlShader = info.shader.tessControl;
    inst.tessEvalShader = info.shader.tessEval;
    inst.geomShader = info.shader.geom;
    inst.fragShader = info.shader.frag;

    inst.info.shader.vert = inst.vertShader.c_str();
    inst.info.shader.tessControl = inst.tessControlShader.c_str();
    inst.info.shader.tessEval = inst.tessEvalShader.c_str();
    inst.info.shader.geom = inst.geomShader.c_str();
    inst.info.shader.frag = inst.fragShader.c_str();

    size_t stride[cMaxVertexAttribs] = { };
    for (size_t i = 0; i < cMaxVertexAttribs; ++i) {
      const char *name = info.attribs[i].name;
      if (name) {
        inst.attributes[i] = name;
        inst.info.attribs[i].name = inst.attributes[i].c_str();
      }
      if (!inst.info.attribs[i].offset) {
        inst.info.attribs[i].offset = stride[inst.info.attribs[i].bufferIndex];
      }
      stride[inst.info.attribs[i].bufferIndex] += RenderContext::computeSize(inst.info.attribs[i].format);
    }

    for (auto &a : inst.info.attribs) {
      if (!a.stride) {
        a.stride = stride[a.bufferIndex];
      }
    }

    ++usedPipelines_;

    return gpu::Pipeline{ctx_, id};
  }

  gpu::Framebuffer GPU::createFramebuffer(const gpu::Framebuffer::Info &info) {
    const uint id = acquireResource(&ctx_->framebuffers_);
    const uint pos = RenderContext::index(id);
    gpu::FramebufferInstance &inst = ctx_->framebuffers_[pos];
    inst.info = info;
    for (uint i = 0; i < info.colorAttachmentsSize; ++i) {
      inst.colorAttachments[i] = createTexture(info.colorAttachmentInfo[i]);
    }
    inst.depthAttachment = createTexture(info.depthStencilAttachmentInfo);

    ++usedFramebuffers_;

    return gpu::Framebuffer{ctx_, id};
  }

}
