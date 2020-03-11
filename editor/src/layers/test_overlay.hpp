//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_TEST_OVERLAY_HPP
#define XE_TEST_OVERLAY_HPP

#include <xe/core/engine.hpp>

class TestOverlay {
public:
  explicit TestOverlay();

  bool onKeyPressed(const xe::Event::Key &e);
};

#endif //XE_TEST_OVERLAY_HPP
