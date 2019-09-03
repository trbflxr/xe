//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/engine.hpp>

namespace xe {

  Engine::Engine() {
    Params::Window wp = {800, 600,
                         "test",
                         true,
                         0, 0, nullptr};

    window_ = make_ref<Window>();
    window_->init(wp);
  }

  void Engine::dummy() {
    while (!window_->shouldClose()) {
      window_->pollEvents();
      window_->swap();
    }
  }

}
