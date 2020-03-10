//
// Created by trbflxr on 2/29/2020.
//

#include "xepch.hpp"
#include <xe/math/vec2.hpp>

#include <xe/math/vec3.hpp>
#include <xe/math/vec4.hpp>

namespace xe {

  template<>
  tvec2<float>::tvec2(const vec3 &vec) : x(vec[0]), y(vec[1]) { }

  template<>
  tvec2<float>::tvec2(const vec4 &vec) : x(vec[0]), y(vec[1]) { }

}
