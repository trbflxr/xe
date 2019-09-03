//
// Created by FLXR on 6/21/2019.
//

#include <xe/math/quat.hpp>
#include <xe/math/vec3.hpp>

namespace xe {

  quat::quat() : data_(VectorConstants::Zero()) { }
  quat::quat(const Vector &quat) : data_(quat) { }
  quat::quat(float x, float y, float z, float w) : data_(Vector::make(x, y, z, w)) { }

  quat::quat(const vec3 &axis, float angleDeg) {
    float values[3];
    float sinAngle;
    float cosAngle;
    sincos(&sinAngle, &cosAngle, -TO_RAD(angleDeg) * 0.5f);
    axis.toVector().store3f(values);

    data_ = Vector::make(values[0] * sinAngle, values[1] * sinAngle, values[2] * sinAngle, cosAngle);
  }

  quat quat::normalize(float errorMargin) const {
    static const Vector defaultQuat = Vector::make(0.0f, 0.0f, 0.0f, 1.0f);

    const Vector lenSq = data_.dot4(data_);
    const Vector mask = lenSq >= Vector::load1f(errorMargin);
    const Vector normalizedVec = data_ * lenSq.rsqrt();

    return quat(normalizedVec.select(mask, defaultQuat));
  }

  bool quat::isNormalized(float errorMargin) const {
    return fabsf(1.0f - lengthSquared()) < errorMargin;
  }

  vec3 quat::rotate(const vec3 &other) const {
    return vec3(data_.quatRotateVec(other.toVector()));
  }

  quat quat::slerp(const quat &dest, float amt, float errorMargin) const {
    const float cosAngleInitial = dot(dest);
    const float cosAngle = select(cosAngleInitial, cosAngleInitial, -cosAngleInitial);

    float lerpAmt1 = 1.0f - amt;
    float lerpAmt2 = amt;
    if (cosAngle < (1.0f - errorMargin)) {
      const float rsinAngle = rsqrt(1.0f - cosAngle * cosAngle);
      const float angle = acosf(cosAngle);

      lerpAmt1 = sinf(lerpAmt1 * angle) * rsinAngle;
      lerpAmt2 = sinf(lerpAmt2 * angle) * rsinAngle;
    }

    lerpAmt2 = select(cosAngleInitial, lerpAmt2, -lerpAmt2);

    const Vector lerpAmt1Vec = Vector::load1f(lerpAmt1);
    const Vector lerpAmt2Vec = Vector::load1f(lerpAmt2);
    return quat(data_ * lerpAmt1Vec + dest.data_ * lerpAmt2Vec);
  }

  quat quat::conjugate() const {
    static const Vector inverter = Vector::make(-1.0f, -1.0f, -1.0f, 1.0f);
    return quat(data_ * inverter);
  }

  quat quat::inverse() const {
    return normalize().conjugate();
  }

  float quat::dot(const quat &other) const {
    return data_.dot4(other.data_)[0];
  }

  float quat::length() const {
    return sqrtf(lengthSquared());
  }

  float quat::lengthSquared() const {
    return data_.dot4(data_)[0];
  }

  vec3 quat::getAxis() const {
    float w = data_[3];
    const float rangleDivisor = rsqrt(max(1.0f - w * w, 0.0f));
    return vec3(data_ * Vector::load1f(rangleDivisor));
  }

  float quat::getAngle() const {
    return TO_DEG(2.0f * acosf(data_[3]));
  }

  void quat::axisAndAngle(vec3 &axis, float &angleDeg) const {
    angleDeg = getAngle();
    axis = getAxis();
  }

  vec3 quat::operator*(const vec3 &other) const {
    return rotate(other);
  }

  vec3 quat::getForward() const {
    return vec3(0.0f, 0.0f, -1.0f).rotate(getAxis(), getAngle());
  }

  vec3 quat::getBackward() const {
    return vec3(0.0f, 0.0f, 1.0f).rotate(getAxis(), getAngle());
  }

  vec3 quat::getUp() const {
    return vec3(0.0f, 1.0f, 0.0f).rotate(getAxis(), getAngle());
  }

  vec3 quat::getDown() const {
    return vec3(0.0f, -1.0f, 0.0f).rotate(getAxis(), getAngle());
  }

  vec3 quat::getRight() const {
    return vec3(1.0f, 0.0f, 0.0f).rotate(getAxis(), getAngle());
  }

  vec3 quat::getLeft() const {
    return vec3(-1.0f, 0.0f, 0.0f).rotate(getAxis(), getAngle());
  }

  vec3 quat::getAxisX() const {
    return rotate(vec3(1.0f, 0.0f, 0.0f));
  }

  vec3 quat::getAxisY() const {
    return rotate(vec3(0.0f, 1.0f, 0.0f));
  }

  vec3 quat::getAxisZ() const {
    return rotate(vec3(0.0f, 0.0f, 1.0f));
  }

  bool quat::equals(const quat &other, float errorMargin) const {
    const Vector error = Vector::load1f(errorMargin);
    return ((data_ - other.data_).abs() >= error).isZero4f() ||
           ((data_ + other.data_).abs() >= error).isZero4f();
  }

}
