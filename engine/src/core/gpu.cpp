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
#include "graphics/display_list_command.hpp"
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

  std::atomic<uint32_t> GPU::gpuCommands_ = 0;

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
    threadSync_.cvL.wait(lock, [this] {
      return threadSync_.initialized;
    });
  }

  void GPU::run() {
    XE_TRACE_META_THREAD_NAME("Render thread");
    window_->init();
    shouldStop_ = window_->shouldClose();

    threadSync_.initialized = true;
    threadSync_.exit = false;
    threadSync_.cvL.notify_one();

    XE_TRACE_BEGIN("XE", "Frame");
    XE_TRACE_BEGIN("XE", "Waiting (render)");

    XE_CORE_INFO("[GPU] Render loop begin");
    while (!shouldStop_) {
      gpu::Backend::drawCalls = 0;

      std::unique_lock<std::mutex> lock(threadSync_.mxR);
      threadSync_.cvR.wait(lock, [this] {
        return renderFrame_.commands_.empty();
      });

      if (!threadSync_.nextFrame.commands_.empty()) {
        XE_TRACE_END("XE", "Waiting (render)");
        renderFrame_.commands_.swap(threadSync_.nextFrame.commands_);

        threadSync_.cvL.notify_one();

        gpuCommands_ = static_cast<uint32_t>(renderFrame_.commands_.size());

        renderFrame_.update();
        window_->update();

        XE_TRACE_END("XE", "Frame");
        XE_TRACE_BEGIN("XE", "Frame");
        XE_TRACE_BEGIN("XE", "Waiting (render)");
      } else {
        threadSync_.cvL.notify_one();
      }

      Event e{ };
      std::vector<Event> events;
      while (window_->pollEvent(e)) {
        events.push_back(e);
      }
      Engine::ref().pushEvents(events);
    }
    XE_CORE_INFO("[GPU] Render loop end");

    XE_TRACE_END("XE", "Frame");
    XE_TRACE_END("XE", "Waiting (render)");

    while (!threadSync_.exit) {
      //wait
    }

    XE_TRACE_BEGIN("XE", "GPU resources cleaning");
    for (auto &&cmd : logicFrame_->commands_) {
      cmd->execute();
    }
    XE_TRACE_END("XE", "GPU resources cleaning");

    window_->stop();
    threadSync_.cvL.notify_one();
  }

  void GPU::prepareRender() {
    XE_TRACE_BEGIN("XE", "Waiting (update)");
    std::unique_lock<std::mutex> lock(threadSync_.mxL);
    threadSync_.cvL.wait(lock, [this] {
      return shouldStop_ || threadSync_.nextFrame.commands_.empty();
    });
    XE_TRACE_END("XE", "Waiting (update)");
  }

  void GPU::execute() {
    if (logicFrame_) {
      threadSync_.nextFrame.commands_ = std::move(logicFrame_->commands_);
    }
    threadSync_.cvR.notify_one();
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
    threadSync_.exit = true;
    threadSync_.thread.join();
    XE_CORE_INFO("[GPU] Joined rendering thread");
  }

  std::shared_ptr<gpu::Buffer> GPU::createBuffer(const gpu::Buffer::Info &info) const {
    const uint32_t id = detail::acquireResource(&ctx_->buffers_);
    if (!id) {
      XE_CORE_CRITICAL("[GPU] Could not create buffer id: '{}'", id);
      return nullptr;
    }

    const uint32_t loc = RenderContext::index(id);
    gpu::BufferInstance &inst = ctx_->buffers_[loc];
    inst.info = info;

    ++usedBuffers_;

    XE_CORE_INFO("[GPU] Created buffer id: {}", id);
    return std::make_shared<gpu::Buffer>(ctx_, id);
  }

  std::shared_ptr<gpu::Texture> GPU::createTexture(const gpu::Texture::Info &info) const {
    auto &&tex = const_cast<gpu::Texture::Info &>(info);

    if (info.format == TexelsFormat::None) {
      XE_CORE_ERROR("[GPU] Could not create texture: texels format not found. Load default!");
      tex = Embedded::defaultTextureInfo();
    }

    const uint32_t id = detail::acquireResource(&ctx_->textures_);
    if (!id) {
      XE_CORE_CRITICAL("[GPU] Could not create texture id: '{}'", id);
      return nullptr;
    }

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

    XE_CORE_INFO("[GPU] Created texture id: {}", id);
    return std::make_shared<gpu::Texture>(ctx_, id);
  }

  std::shared_ptr<gpu::Pipeline> GPU::createPipeline(const gpu::Pipeline::Info &info) const {
    const uint32_t id = detail::acquireResource(&ctx_->pipelines_);
    if (!id) {
      XE_CORE_CRITICAL("[GPU] Could not create pipeline id: '{}'", id);
      return nullptr;
    }

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

    XE_CORE_INFO("[GPU] Created pipeline id: {}", id);
    return std::make_shared<gpu::Pipeline>(ctx_, id);
  }

  std::shared_ptr<gpu::Framebuffer> GPU::createFramebuffer(const gpu::Framebuffer::Info &info) const {
    const uint32_t id = detail::acquireResource(&ctx_->framebuffers_);
    if (!id) {
      XE_CORE_CRITICAL("[GPU] Could not create framebuffer id: '{}'", id);
      return nullptr;
    }

    const uint32_t pos = RenderContext::index(id);
    gpu::FramebufferInstance &inst = ctx_->framebuffers_[pos];
    inst.info = info;
    for (uint32_t i = 0; i < info.colorAttachmentsSize; ++i) {
      inst.colorAttachments[i] = createTexture(info.colorAttachmentInfo[i]);

      //clear color textures if creation failed
      if (!inst.colorAttachments[i]) {
        XE_CORE_CRITICAL("[GPU] Could not create framebuffer id: '{}'. Color texture {} creation failed", id, i);
        for (uint32_t j = i - 1; j >= 0; --j) {
          destroyResource(*inst.colorAttachments[i]);
        }
        return nullptr;
      }
    }

    inst.depthAttachment = createTexture(info.depthStencilAttachmentInfo);
    if (!inst.depthAttachment) {
      XE_CORE_CRITICAL("[GPU] Could not create framebuffer id: '{}'. Depth texture creation failed", id);
      return nullptr;
    }

    ++usedFramebuffers_;

    XE_CORE_INFO("[GPU] Created framebuffer id: {}", id);
    return std::make_shared<gpu::Framebuffer>(ctx_, id);
  }

  void GPU::destroyResource(const gpu::Resource &resource) const {
    auto cmd = std::make_shared<DestroyCommand>();
    cmd->data_.resource = resource;

    if (logicFrame_->commands_.empty()) {
      logicFrame_->commands_.emplace_back(std::move(cmd));
    } else {
      logicFrame_->commands_.reserve(logicFrame_->commands_.size() + 1);
      logicFrame_->commands_.emplace_back(std::move(cmd));
    }
  }

  uint32_t GPU::drawCalls() {
    return gpu::Backend::drawCalls;
  }

  uint32_t GPU::gpuCommands() {
    return gpuCommands_;
  }

}
