//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "draw_list_command.hpp"
#include <xe/graphics/draw_list.hpp>
#include <xe/utils/logger.hpp>
#include <xe/utils/debug_trace.hpp>

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

  DrawList::ClearData &DrawList::clearCommand() {
    ClearCommand *cmd = new ClearCommand();
    ref_ptr<Command> c;
    c.reset(cmd);
    commands_.push_back(c);
    return cmd->data_;
  }

}
