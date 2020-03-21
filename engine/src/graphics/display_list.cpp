//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/graphics/display_list.hpp>

#ifdef XE_PLATFORM_GL
  #include "graphics/opengl/gl_backend.hpp"
#else
  #error only gl currently supported
#endif

namespace xe {

  DisplayList::DisplayList() {
    setName("DisplayList");
  }

  void DisplayList::update() {
    XE_TRACE_BEGIN("XE", "Draw list update");
    for (auto &&cmd : commands_) {
      cmd.execute();
    }
    commands_.clear();
    XE_TRACE_END("XE", "Draw list update");
  }

  void DisplayList::Command::execute() {
    switch (type_) {
      case View: {
        XE_TRACE_BEGIN("XE", "Setup view");
        gpu::Backend::setupView(viewData_);
        XE_TRACE_END("XE", "Setup view");
        break;
      }
      case Clear: {
        XE_TRACE_BEGIN("XE", "Clear");
        gpu::Backend::clear(clearData_);
        XE_TRACE_END("XE", "Clear");
        break;
      }
      case Buffer: {
        XE_TRACE_BEGIN("XE", "Fill Buffer");
        gpu::Backend::fillBuffer(fillBufferData_);
        XE_TRACE_END("XE", "Fill Buffer");
        break;
      }
      case Texture: {
        XE_TRACE_BEGIN("XE", "Fill Texture");
        gpu::Backend::fillTexture(fillTextureData_);
        XE_TRACE_END("XE", "Fill Texture");
        break;
      }
      case Pipeline: {
        XE_TRACE_BEGIN("XE", "Setup Pipeline");
        gpu::Backend::setupPipeline(setupPipelineData_);
        XE_TRACE_END("XE", "Setup Pipeline");
        break;
      }
      case Render: {
        XE_TRACE_BEGIN("XE", "Render");
        gpu::Backend::render(renderData_);
        XE_TRACE_END("XE", "Render");
        break;
      }
      case Destroy: {
        XE_TRACE_BEGIN("XE", "Destroy");
        gpu::Backend::destroy(destroyData_);
        XE_TRACE_END("XE", "Destroy");
        break;
      }
      case Invalid:
      default: {
        break;
      }
    }
  }

  DisplayList::ViewData &DisplayList::setupViewCommand() {
    commands_.emplace_back(Command::Type::View);
    return commands_.back().viewData_;
  }

  DisplayList::ClearData &DisplayList::clearCommand() {
    commands_.emplace_back(Command::Type::Clear);
    return commands_.back().clearData_;
  }

  DisplayList::FillBufferData &DisplayList::fillBufferCommand() {
    commands_.emplace_back(Command::Type::Buffer);
    return commands_.back().fillBufferData_;
  }

  DisplayList::FillTextureData &DisplayList::fillTextureCommand() {
    commands_.emplace_back(Command::Type::Texture);
    return commands_.back().fillTextureData_;
  }

  DisplayList::SetupPipelineData &DisplayList::setupPipelineCommand() {
    commands_.emplace_back(Command::Type::Pipeline);
    return commands_.back().setupPipelineData_;
  }

  DisplayList::RenderData &DisplayList::renderCommand() {
    commands_.emplace_back(Command::Type::Render);
    return commands_.back().renderData_;
  }

}
