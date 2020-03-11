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
  if (e.code == Keyboard::T) {
    static std::string title = std::string(Engine::ref().window().getTitle());
    title += std::to_string(e.code);
    Engine::ref().window().setTitle(title);
  }
//  XE_INFO("[TestOverlay] key pressed ({})", e.code);
  return false;
}
