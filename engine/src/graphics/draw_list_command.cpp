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

  void SetupViewCommand::execute() {
    XE_TRACE_BEGIN("XE", "Setup view");
    gpu::BackEnd::setupView(data_);
    XE_TRACE_END("XE", "Setup view");
  }

  void ClearCommand::execute() {
    XE_TRACE_BEGIN("XE", "Clear");
    gpu::BackEnd::clear(data_);
    XE_TRACE_END("XE", "Clear");
  }

  void FillBufferCommand::execute() {
    XE_TRACE_BEGIN("XE", "Fill Buffer");
    gpu::BackEnd::fillBuffer(data_);
    XE_TRACE_END("XE", "Fill Buffer");
  }

  void FillTextureCommand::execute() {
    XE_TRACE_BEGIN("XE", "Fill Texture");
    gpu::BackEnd::fillTexture(data_);
    XE_TRACE_END("XE", "Fill Texture");
  }

  void SetupMaterialCommand::execute() {
    XE_TRACE_BEGIN("XE", "Setup Material");
    gpu::BackEnd::setupMaterial(data_);
    XE_TRACE_END("XE", "Setup Material");
  }

  void RenderCommand::execute() {
    XE_TRACE_BEGIN("XE", "Render");
    gpu::BackEnd::render(data_);
    XE_TRACE_END("XE", "Render");
  }

}
