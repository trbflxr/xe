//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "draw_list_command.hpp"
#include <xe/graphics/draw_list.hpp>

namespace xe {

  DrawList::DrawList() {
    setName("DrawList");
  }

  void DrawList::update() {
    XE_TRACE_BEGIN("XE", "Draw list update");
    XE_CORE_TRACE("[DrawList] Executing DrawList (Commands {})", commands_.size());
    for (auto &&cmd : commands_) {
      cmd->execute();
    }
    commands_.clear();
    XE_TRACE_END("XE", "Draw list update");
  }

  DrawList::ViewData &DrawList::setupViewCommand() {
    SetupViewCommand *cmd = new SetupViewCommand();
    ref_ptr<SetupViewCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DrawList::ClearData &DrawList::clearCommand() {
    ClearCommand *cmd = new ClearCommand();
    ref_ptr<ClearCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DrawList::FillBufferData &DrawList::fillBufferCommand() {
    FillBufferCommand *cmd = new FillBufferCommand();
    ref_ptr<FillBufferCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DrawList::FillTextureData &DrawList::fillTextureCommand() {
    FillTextureCommand *cmd = new FillTextureCommand();
    ref_ptr<FillTextureCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DrawList::SetupMaterialData &DrawList::setupMaterialCommand() {
    SetupMaterialCommand *cmd = new SetupMaterialCommand();
    ref_ptr<SetupMaterialCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

  DrawList::RenderData &DrawList::renderCommand() {
    RenderCommand *cmd = new RenderCommand();
    ref_ptr<RenderCommand> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

}
