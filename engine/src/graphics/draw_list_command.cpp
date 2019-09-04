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
    XE_TRACE_SCOPE("XE", "Clear");
    gpu::clear(data_);
  }

  void FillBufferCommand::execute() {
    XE_TRACE_SCOPE("XE", "Fill Buffer");
    gpu::fillBuffer(data_);
  }

  void RenderCommand::execute() {
    XE_TRACE_SCOPE("XE", "Render");
    gpu::render(data_);
  }

}
