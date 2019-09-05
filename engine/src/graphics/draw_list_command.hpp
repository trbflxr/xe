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

  struct SetupViewCommand : DrawList::Command {
    friend class DrawList;
    void execute() override;

    DrawList::ViewData data_;
  };

  struct ClearCommand : DrawList::Command {
    friend class DrawList;
    void execute() override;

    DrawList::ClearData data_;
  };

  struct FillBufferCommand : DrawList::Command {
    friend class DrawList;
    void execute() override;

    DrawList::FillBufferData data_;
  };

  struct FillTextureCommand : DrawList::Command {
    friend class DrawList;
    void execute() override;

    DrawList::FillTextureData data_;
  };

  struct SetupMaterialCommand : DrawList::Command {
    friend class DrawList;
    void execute() override;

    DrawList::SetupMaterialData data_;
  };

  struct RenderCommand : DrawList::Command {
    friend class DrawList;
    void execute() override;

    DrawList::RenderData data_;
  };

}

#endif //XE_DRAW_LIST_COMMAND_HPP
