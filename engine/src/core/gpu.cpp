//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/gpu.hpp>

#ifdef XE_PLATFORM_GL
  #include "graphics/opengl/gl_backend.hpp"
#else
  #error only gl currently supported
#endif

#include "embedded/embedded.hpp"
#include <xe/core/engine.hpp>
#include <xe/graphics/window.hpp>
#include <xe/graphics/render_context.hpp>

namespace xe::detail {

  template<class T>
  static uint32_t acquireResource(memory<T> *pool) {
    uint32_t tryCount = 10;
    while (tryCount--) {
      for (uint32_t i = 0; i < pool->size; ++i) {
        if ((*pool)[i].acquire()) {
          const uint32_t version = (*pool)[i].version;
          const uint32_t result = i | (version << 20);
          return result;
        }
      }
    }
    XE_CORE_ERROR("[GPU] Assigned id 0 to resource instance");
    return 0;
  }

}

namespace xe {

  GPU::GPU() :
      window_(std::make_shared<Window>()),
      logicFrame_(std::make_unique<DisplayList>()),
      ctx_(new RenderContext()) {
    setName("GPU");
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
    shouldClose_ = window_->shouldClose();

    threadSync_.initialized = true;
    threadSync_.cvL.notify_one();

    XE_TRACE_BEGIN("XE", "Frame");
    XE_TRACE_BEGIN("XE", "Waiting (render)");

    while (!shouldClose_) {
      shouldClose_ = window_->shouldClose();

      gpu::Backend::drawCalls = 0;

      XE_CORE_TRACE("[GPU] GPU Synchronization (render waiting)");
      std::unique_lock<std::mutex> lock(threadSync_.mxR);
      threadSync_.cvR.wait(lock, [this] { return renderFrame_.commands_.empty(); });

      if (!threadSync_.nextFrame.commands_.empty()) {
        XE_TRACE_END("XE", "Waiting (render)");
        XE_CORE_TRACE("[GPU] GPU Synchronization (render start)");
        renderFrame_.commands_.swap(threadSync_.nextFrame.commands_);

        threadSync_.cvL.notify_one();
        XE_CORE_TRACE("[GPU] GPU Synchronization (render ready)");

        renderFrame_.update();
        window_->update();


        Event e{ };
        std::vector<Event> events;
        while (window_->pollEvent(e)) {
          events.push_back(e);
        }
        Engine::ref().pushEvents(events);

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
    threadSync_.cvL.wait(lock, [this] { return shouldClose_ || threadSync_.nextFrame.commands_.empty(); });
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

  gpu::Buffer GPU::createBuffer(const gpu::Buffer::Info &info) const {
    const uint32_t id = detail::acquireResource(&ctx_->buffers_);
    const uint32_t loc = RenderContext::index(id);
    gpu::BufferInstance &inst = ctx_->buffers_[loc];
    inst.info = info;

    ++usedBuffers_;

    return gpu::Buffer(ctx_, id);
  }

  gpu::Texture GPU::createTexture(const gpu::Texture::Info &info) const {
    auto &&tex = const_cast<gpu::Texture::Info &>(info);

    if (info.format == TexelsFormat::None) {
      XE_CORE_ERROR("[GPU] Could not create texture: texels format not found. Load default!");
      tex = Embedded::defaultTextureInfo();
    }

    const uint32_t id = detail::acquireResource(&ctx_->textures_);
    const uint32_t loc = RenderContext::index(id);
    gpu::TextureInstance &inst = ctx_->textures_[loc];
    inst.info = tex;

    switch (tex.format) {
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

  gpu::Pipeline GPU::createPipeline(const gpu::Pipeline::Info &info) const {
    const uint32_t id = detail::acquireResource(&ctx_->pipelines_);
    const uint32_t loc = RenderContext::index(id);
    gpu::PipelineInstance &inst = ctx_->pipelines_[loc];
    inst.info = info;

    inst.vertShader = info.shader.vert;
    inst.tessControlShader = info.shader.tessControl;
    inst.tessEvalShader = info.shader.tessEval;
    inst.geomShader = info.shader.geom;
    inst.fragShader = info.shader.frag;

    inst.info.shader.vert = inst.vertShader;
    inst.info.shader.tessControl = inst.tessControlShader;
    inst.info.shader.tessEval = inst.tessEvalShader;
    inst.info.shader.geom = inst.geomShader;
    inst.info.shader.frag = inst.fragShader;

    uint32_t stride[cMaxVertexAttribs] = { };
    for (uint32_t i = 0; i < cMaxVertexAttribs; ++i) {
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

  gpu::Framebuffer GPU::createFramebuffer(const gpu::Framebuffer::Info &info) const {
    const uint32_t id = detail::acquireResource(&ctx_->framebuffers_);
    const uint32_t pos = RenderContext::index(id);
    gpu::FramebufferInstance &inst = ctx_->framebuffers_[pos];
    inst.info = info;
    for (uint32_t i = 0; i < info.colorAttachmentsSize; ++i) {
      inst.colorAttachments[i] = createTexture(info.colorAttachmentInfo[i]);
    }
    inst.depthAttachment = createTexture(info.depthStencilAttachmentInfo);

    ++usedFramebuffers_;

    return gpu::Framebuffer{ctx_, id};
  }

  uint32_t GPU::drawCalls() {
    return gpu::Backend::drawCalls;
  }

  uint32_t GPU::gpuCommands() {
    return gpu::Backend::gpuCommands;
  }

}
