//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_ENGINE_HPP
#define XE_ENGINE_HPP

#include <xe/memory.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/window.hpp>

namespace xe {

  class XE_API Engine : public Object {
  XE_OBJECT(Engine, Object);
  public:
    Engine();

    void dummy();

    Window &window() { return *window_; }

  private:
    ref_ptr<Window> window_;

  };

}


#endif //XE_ENGINE_HPP
