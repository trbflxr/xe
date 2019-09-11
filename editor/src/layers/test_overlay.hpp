//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_TEST_OVERLAY_HPP
#define XE_TEST_OVERLAY_HPP

#include <xe/core/layer.hpp>

class TestOverlay : public xe::Layer {
public:
  explicit TestOverlay(xe::Application &app);

  bool onKeyPressed(xe::Event::Key e) override;
};

#endif //XE_TEST_OVERLAY_HPP
