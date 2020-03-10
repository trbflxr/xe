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
    XE_CORE_TRACE("[DisplayList] Executing DisplayList (Commands {})", commands_.size());
    for (auto &&cmd : commands_) {
      cmd->execute();
    }
    commands_.clear();
    XE_TRACE_END("XE", "Draw list update");
  }

  DisplayList::ViewData &DisplayList::setupViewCommand() {
    SetupViewCommand *cmd = new SetupViewCommand();
    std::shared_ptr<SetupViewCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DisplayList::ClearData &DisplayList::clearCommand() {
    ClearCommand *cmd = new ClearCommand();
    std::shared_ptr<ClearCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DisplayList::FillBufferData &DisplayList::fillBufferCommand() {
    FillBufferCommand *cmd = new FillBufferCommand();
    std::shared_ptr<FillBufferCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DisplayList::FillTextureData &DisplayList::fillTextureCommand() {
    FillTextureCommand *cmd = new FillTextureCommand();
    std::shared_ptr<FillTextureCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DisplayList::SetupPipelineData &DisplayList::setupPipelineCommand() {
    SetupPipelineCommand *cmd = new SetupPipelineCommand();
    std::shared_ptr<SetupPipelineCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DisplayList::RenderData &DisplayList::renderCommand() {
    RenderCommand *cmd = new RenderCommand();
    std::shared_ptr<RenderCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

}
