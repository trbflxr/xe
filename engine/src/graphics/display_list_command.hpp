//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_DISPLAY_LIST_COMMAND_HPP
#define XE_DISPLAY_LIST_COMMAND_HPP

#include <xe/graphics/display_list.hpp>

namespace xe {

  struct DisplayList::Command {
    virtual void execute() = 0;
  };

  struct SetupViewCommand : DisplayList::Command {
    friend class DisplayList;
    void execute() override;

    DisplayList::ViewData data_;
  };

  struct ClearCommand : DisplayList::Command {
    friend class DisplayList;
    void execute() override;

    DisplayList::ClearData data_;
  };

  struct FillBufferCommand : DisplayList::Command {
    friend class DisplayList;
    void execute() override;

    DisplayList::FillBufferData data_;
  };

  struct FillTextureCommand : DisplayList::Command {
    friend class DisplayList;
    void execute() override;

    DisplayList::FillTextureData data_;
  };

  struct SetupPipelineCommand : DisplayList::Command {
    friend class DisplayList;
    void execute() override;

    DisplayList::SetupPipelineData data_;
  };

  struct RenderCommand : DisplayList::Command {
    friend class DisplayList;
    void execute() override;

    DisplayList::RenderData data_;
  };

}

#endif //XE_DISPLAY_LIST_COMMAND_HPP
