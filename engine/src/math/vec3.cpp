//
// Created by FLXR on 6/14/2019.
//

#include <xe/math/vec3.hpp>
#include <xe/math/vec2.hpp>
#include <xe/math/vec4.hpp>
#include <xe/math/mathfunc.hpp>

namespace xe {

  vec3::vec3(float x, float y, float z) : data_(Vector::make(x, y, z, 0.0f)) { }
  vec3::vec3() : data_(VectorConstants::Zero()) { }
  vec3::vec3(float val) : data_(Vector::make(val, val, val, 0.0f)) { }
  vec3::vec3(const vec4 &vec) : data_(Vector::make(vec[0], vec[1], vec[2], 0.0f)) { }
  vec3::vec3(const Vector &vecIn) : data_(vecIn) { }

  void vec3::set(float x, float y, float z) {
    data_ = Vector::make(x, y, z, 0.0f);
  }

  void vec3::set(uint index, float val) {
    data_ = data_.select(Vector::mask(index), Vector::load1f(val));
  }

  vec3 vec3::normalize(float errorMargin) const {
    (void) errorMargin;
    return vec3(data_.normalize3());
  }

  bool vec3::isNormalized(float errorMargin) const {
    return fabsf(1.0f - lengthSquared()) < errorMargin;
  }

  float vec3::max() const {
    float values[3];
    data_.store3f(values);
    return math::max3(values[0], values[1], values[2]);
  }

  float vec3::min() const {
    float values[3];
    data_.store3f(values);
    return math::min3(values[0], values[1], values[2]);
  }

  float vec3::absMax() const {
    float values[3];
    data_.abs().store3f(values);
    return math::max3(values[0], values[1], values[2]);
  }

  float vec3::absMin() const {
    float values[3];
    data_.abs().store3f(values);
    return math::min3(values[0], values[1], values[2]);
  }

  vec3 vec3::abs() const {
    return vec3(data_.abs());
  }

  vec3 vec3::min(const vec3 &other) const {
    return vec3(data_.min(other.data_));
  }

  vec3 vec3::max(const vec3 &other) const {
    return vec3(data_.max(other.data_));
  }

  void vec3::dirAndLength(vec3 &dir, float &length) const {
    const Vector rlen = data_.dot3(data_).rsqrt();
    dir = vec3(data_ * rlen);
    length = math::reciprocal(rlen[0]);
  }

  vec3 vec3::project() const {
    Vector rprojectVal = Vector::load1f(math::reciprocal(data_[2]));
    return vec3(data_ * rprojectVal);
  }

  vec3 vec3::reciprocal() const {
    return vec3(data_.reciprocal());
  }

  vec3 vec3::rotate(const vec3 &axis, float angle) const {
    float sinAngle;
    float cosAngle;

    math::sincos(&sinAngle, &cosAngle, -angle);
    const Vector sinVec = Vector::load1f(sinAngle);
    const Vector cosVec = Vector::load1f(cosAngle);
    const Vector oneMinusCosVec = Vector::load1f(1.0f - cosAngle);

    const Vector rotatedX = data_.cross3(axis.data_ * sinVec);
    const Vector rotatedY = axis.data_ * data_.dot3(axis.data_ * oneMinusCosVec);
    const Vector rotatedZ = data_ * cosVec;

    return vec3(rotatedX + rotatedY + rotatedZ);
  }

  vec3 vec3::reflect(const vec3 &normal) const {
    const Vector dotAmt = VectorConstants::Two() * data_.dot3(normal.data_);
    return vec3(data_ - (normal.data_ * dotAmt));
  }

  vec3 vec3::refract(const vec3 &normal, float indexOfRefraction) const {
    const float cosNormalAngle = data_.dot3(normal.data_)[0];
    const float rs = 1.0f - indexOfRefraction * indexOfRefraction * (1.0f - cosNormalAngle * cosNormalAngle);

    if (rs < 0.0f) {
      return vec3(VectorConstants::Zero());
    }

    const float normalScale = indexOfRefraction * cosNormalAngle + sqrt(rs);
    const Vector normalScaleVec(Vector::load1f(normalScale));
    const Vector indexOfRefractionVec(Vector::load1f(indexOfRefraction));

    return vec3(data_ * indexOfRefractionVec - normalScaleVec * normal.data_);
  }

  float vec3::distSquared(const vec3 &other) const {
    vec3 temp = other - *this;
    return temp.data_.dot3(temp.data_)[0];
  }

  vec3 vec3::toDeg() const {
    return vec3(data_ * Vector::load1f(XE_MATH_RAD_TO_DEG));
  }

  vec3 vec3::toRad() const {
    return vec3(data_ * Vector::load1f(XE_MATH_DEG_TO_RAD));
  }

  bool vec3::equals(const vec3 &other, float errorMargin) const {
    return data_.notEquals(other.data_, errorMargin).isZero3f();
  }

  bool vec3::equals(float val, float errorMargin) const {
    return equals(vec3(val), errorMargin);
  }

  const vec3 &vec3::unitX() {
    static const vec3 u = vec3(1.0f, 0.0f, 0.0f);
    return u;
  }

  const vec3 &vec3::unitY() {
    static const vec3 u = vec3(0.0f, 1.0f, 0.0f);
    return u;
  }

  const vec3 &vec3::unitZ() {
    static const vec3 u = vec3(0.0f, 0.0f, 1.0f);
    return u;
  }

  const vec3 &vec3::unitXN() {
    static const vec3 u = vec3(-1.0f, 0.0f, 0.0f);
    return u;
  }

  const vec3 &vec3::unitYN() {
    static const vec3 u = vec3(0.0f, -1.0f, 0.0f);
    return u;
  }

  const vec3 &vec3::unitZN() {
    static const vec3 u = vec3(0.0f, 0.0f, -1.0f);
    return u;
  }

  const vec3 &vec3::zero() {
    static const vec3 u = vec3(0.0f, 0.0f, 0.0f);
    return u;
  }

}
