//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "display_list_command.hpp"
#include <xe/graphics/display_list.hpp>

namespace xe {

  DisplayList::DisplayList() {
    setName("DisplayList");
  }

  void DisplayList::update() {
    XE_TRACE_BEGIN("XE", "Draw list update");
    for (auto &&cmd : commands_) {
      cmd->execute();
    }
    commands_.clear();
    XE_TRACE_END("XE", "Draw list update");
  }

  DisplayList::ViewData &DisplayList::setupViewCommand() {
    commands_.emplace_back(std::make_shared<SetupViewCommand>());
    return dynamic_cast<SetupViewCommand &>(*commands_.back()).data_;
  }

  DisplayList::ClearData &DisplayList::clearCommand() {
    commands_.emplace_back(std::make_shared<ClearCommand>());
    return dynamic_cast<ClearCommand &>(*commands_.back()).data_;
  }

  DisplayList::FillBufferData &DisplayList::fillBufferCommand() {
    commands_.emplace_back(std::make_shared<FillBufferCommand>());
    return dynamic_cast<FillBufferCommand &>(*commands_.back()).data_;
  }

  DisplayList::FillTextureData &DisplayList::fillTextureCommand() {
    commands_.emplace_back(std::make_shared<FillTextureCommand>());
    return dynamic_cast<FillTextureCommand &>(*commands_.back()).data_;
  }

  DisplayList::SetupPipelineData &DisplayList::setupPipelineCommand() {
    commands_.emplace_back(std::make_shared<SetupPipelineCommand>());
    return dynamic_cast<SetupPipelineCommand &>(*commands_.back()).data_;
  }

  DisplayList::RenderData &DisplayList::renderCommand() {
    commands_.emplace_back(std::make_shared<RenderCommand>());
    return dynamic_cast<RenderCommand &>(*commands_.back()).data_;
  }

}
