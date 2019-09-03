//
// Created by FLXR on 6/14/2019.
//

#include <xe/math/vec2.hpp>
#include <xe/math/vec3.hpp>
#include <xe/math/vec4.hpp>

namespace xe {

  vec2::vec2(float x, float y) : x(x), y(y) { }
  vec2::vec2() : x(0.0f), y(0.0f) { }
  vec2::vec2(float value) : x(value), y(value) { }
  vec2::vec2(const vec3 &vec) : x(vec[0]), y(vec[1]) { }
  vec2::vec2(const vec4 &vec) : x(vec[0]), y(vec[1]) { }

  vec2 vec2::normalize(float errorMargin) const {
    const float lenSq = lengthSquared();
    if (lenSq < errorMargin) {
      return vec2(0.0f, 0.0f);
    }
    return (*this) * rsqrt(lenSq);
  }

  bool vec2::isNormalized(float errorMargin) const {
    return fabsf(1.0f - lengthSquared()) < errorMargin;
  }

  float vec2::max() const {
    return xe::max(x, y);
  }

  float vec2::min() const {
    return xe::min(x, y);
  }

  float vec2::absMax() const {
    return xe::max(fabsf(x), fabsf(y));
  }

  float vec2::absMin() const {
    return xe::min(fabsf(x), fabsf(y));
  }

  vec2 vec2::abs() const {
    return vec2(fabsf(x), fabsf(y));
  }

  vec2 vec2::min(const vec2 &other) const {
    return vec2(xe::min(x, other.x), xe::min(y, other.y));
  }

  vec2 vec2::max(const vec2 &other) const {
    return vec2(xe::max(x, other.x), xe::max(y, other.y));
  }

  void vec2::dirAndLength(vec2 &dir, float &length, float errorMargin) const {
    const float lenSq = lengthSquared();
    if (lenSq < errorMargin) {
      dir = vec2(0.0f, 0.0f);
      length = 0;
      return;
    }
    const float rlen = rsqrt(lenSq);
    dir = (*this) * rlen;
    length = xe::reciprocal(rlen);
  }

  vec2 vec2::reciprocal() const {
    return vec2(xe::reciprocal(x), xe::reciprocal(y));
  }

  vec2 vec2::rotate(float angle) const {
    float sin;
    float cos;
    sincos(&sin, &cos, angle);
    return vec2(cos * x - sin * y, sin * x + cos * y);
  }

  vec2 vec2::reflect(const vec2 &normal) const {
    const vec2 dotAmt = vec2(2.0f * dot(normal));
    return (*this) - (normal * dotAmt);
  }

  vec2 vec2::refract(const vec2 &normal, float indexOfRefraction) const {
    const float cosNormalAngle = dot(normal);
    const float refSq = 1.0f - indexOfRefraction * indexOfRefraction * (1.0f - cosNormalAngle * cosNormalAngle);

    if (refSq < 0.0f) {
      return vec2(0.0f);
    }

    const float normalScale = indexOfRefraction * cosNormalAngle + sqrtf(refSq);
    const vec2 normalScaleVec(normalScale);
    const vec2 indexOfRefractionVec(indexOfRefraction);

    return (*this) * indexOfRefractionVec - normalScaleVec * normal;
  }

  vec2 vec2::toDeg() const {
    return vec2(XE_MATH_RAD_TO_DEG * x, XE_MATH_RAD_TO_DEG * y);
  }

  vec2 vec2::toRad() const {
    return vec2(XE_MATH_DEG_TO_RAD * x, XE_MATH_DEG_TO_RAD * y);
  }

  bool vec2::equals(const vec2 &other, float errorMargin) const {
    return (x - other.x) < errorMargin && (y - other.y) < errorMargin;
  }

  bool vec2::equals(float val, float errorMargin) const {
    return (x - val) < errorMargin && (y - val) < errorMargin;
  }

}
