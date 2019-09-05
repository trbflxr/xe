//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "draw_list_command.hpp"
#ifdef XE_PLATFORM_GL
  #include "graphics/opengl/gl_backend.hpp"
#else
  #error only gl currently supported
#endif

namespace xe {

  void ClearCommand::execute() {
    XE_TRACE_BEGIN("XE", "Clear");
    gpu::clear(data_);
    XE_TRACE_END("XE", "Clear");
  }

  void FillBufferCommand::execute() {
    XE_TRACE_BEGIN("XE", "Fill Buffer");
    gpu::fillBuffer(data_);
    XE_TRACE_END("XE", "Fill Buffer");
  }

  void FillTextureCommand::execute() {
    XE_TRACE_BEGIN("XE", "Fill Texture");
    gpu::fillTexture(data_);
    XE_TRACE_END("XE", "Fill Texture");
  }

  void SetupMaterialCommand::execute() {
    XE_TRACE_BEGIN("XE", "Setup Material");
    gpu::setupMaterial(data_);
    XE_TRACE_END("XE", "Setup Material");
  }

  void RenderCommand::execute() {
    XE_TRACE_BEGIN("XE", "Render");
    gpu::render(data_);
    XE_TRACE_END("XE", "Render");
  }

}
