//
// Created by FLXR on 9/11/2019.
//

#include "test_overlay.hpp"
#include <xe/utils/logger.hpp>

using namespace xe;


TestOverlay::TestOverlay(xe::Application &app) :
    Layer(app) {

}

bool TestOverlay::onKeyPressed(const Event::Key &e) {
//  XE_INFO("[TestOverlay] key pressed ({})", e.code);
  return false;
}
