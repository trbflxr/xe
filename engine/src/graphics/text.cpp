//
// Created by trbflxr on 3/23/2020.
//

#include "xepch.hpp"
#include <xe/graphics/text.hpp>

namespace xe {

  Text::Text() {
    setName("Text");
  }

  Text::Text(std::string_view string, const std::shared_ptr<Font> &font, const vec2 &position, float size) :
      font_(font),
      string_(string),
      position_(position),
      size_(size) {
    setName("Text");
  }

}
