//
// Created by FLXR on 6/15/2019.
//

#include <xe/math/vec4.hpp>
#include <xe/math/vec3.hpp>
#include <xe/math/vec2.hpp>

namespace xe {

  vec4::vec4(float x, float y, float z, float w) : data_(Vector::make(x, y, z, 0.0f)) { }
  vec4::vec4(const vec2 &vec) : data_(Vector::make(vec.x, vec.y, 0.0f, 0.0f)) { }
  vec4::vec4(const vec3 &vec) : data_(Vector::make(vec[0], vec[1], vec[2], 0.0f)) { }
  vec4::vec4() : data_(VectorConstants::Zero()) { }
  vec4::vec4(float val) : data_(Vector::make(val, val, val, val)) { }
  vec4::vec4(const Vector &vecIn) : data_(vecIn) { }

  void vec4::set(float x, float y, float z, float w) {
    data_ = Vector::make(x, y, z, w);
  }

  void vec4::set(uint index, float val) {
    data_ = data_.select(Vector::mask(index), Vector::load1f(val));
  }

  bool vec4::equals(const vec4 &other, float errorMargin) const {
    return data_.notEquals(other.data_, errorMargin).isZero4f();
  }

  bool vec4::equals(float val, float errorMargin) const {
    return equals(vec4(val), errorMargin);
  }

}
