//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "display_list_command.hpp"
#ifdef XE_PLATFORM_GL
  #include "graphics/opengl/gl_backend.hpp"
#else
  #error only gl currently supported
#endif

namespace xe {

  void SetupViewCommand::execute() {
    XE_TRACE_BEGIN("XE", "Setup view");
    gpu::Backend::setupView(data_);
    XE_TRACE_END("XE", "Setup view");
  }

  void ClearCommand::execute() {
    XE_TRACE_BEGIN("XE", "Clear");
    gpu::Backend::clear(data_);
    XE_TRACE_END("XE", "Clear");
  }

  void FillBufferCommand::execute() {
    XE_TRACE_BEGIN("XE", "Fill Buffer");
    gpu::Backend::fillBuffer(data_);
    XE_TRACE_END("XE", "Fill Buffer");
  }

  void FillTextureCommand::execute() {
    XE_TRACE_BEGIN("XE", "Fill Texture");
    gpu::Backend::fillTexture(data_);
    XE_TRACE_END("XE", "Fill Texture");
  }

  void SetupPipelineCommand::execute() {
    XE_TRACE_BEGIN("XE", "Setup Pipeline");
    gpu::Backend::setupPipeline(data_);
    XE_TRACE_END("XE", "Setup Pipeline");
  }

  void RenderCommand::execute() {
    XE_TRACE_BEGIN("XE", "Render");
    gpu::Backend::render(data_);
    XE_TRACE_END("XE", "Render");
  }

  void xe::DestroyCommand::execute() {
    XE_TRACE_BEGIN("XE", "Destroy");
    gpu::Backend::destroy(data_);
    XE_TRACE_END("XE", "Destroy");
  }

}
