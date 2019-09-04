//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_DRAW_LIST_COMMAND_HPP
#define XE_DRAW_LIST_COMMAND_HPP

#include <xe/graphics/draw_list.hpp>

namespace xe {

  struct DrawList::Command {
    virtual void execute() = 0;
  };

  struct ClearCommand : DrawList::Command {
    friend class DisplayList;
    void execute() override;

    DrawList::ClearData data_;
  };

}

#endif //XE_DRAW_LIST_COMMAND_HPP
