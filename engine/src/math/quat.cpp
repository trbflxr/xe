//
// Created by FLXR on 6/21/2019.
//

#include <xe/math/quat.hpp>
#include <xe/math/vec3.hpp>

namespace xe {

  quat::quat() : data_(VectorConstants::Zero()) { }
  quat::quat(const Vector &quat) : data_(quat) { }
  quat::quat(float x, float y, float z, float w) : data_(Vector::make(x, y, z, w)) { }

  quat::quat(const vec3 &eulerAngle) {
    float c[3];
    float s[3];
    for (byte i = 0; i < 3; ++i) {
      c[i] = cosf(TO_RAD(eulerAngle[i]) * 0.5f);
      s[i] = sinf(TO_RAD(eulerAngle[i]) * 0.5f);
    }
    data_ = Vector::make(-s[0] * c[1] * c[2] - c[0] * s[1] * s[2],
                         -c[0] * s[1] * c[2] + s[0] * c[1] * s[2],
                         -c[0] * c[1] * s[2] - s[0] * s[1] * c[2],
                         c[0] * c[1] * c[2] + s[0] * s[1] * s[2]);
  }

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

  vec3 quat::toEulerAngles() const {
    const float sinrCosp = 2.0f * (data_[3] * data_[0] + data_[1] * data_[2]);
    const float cosrCosp = 1.0f - 2.0f * (data_[0] * data_[0] + data_[1] * data_[1]);
    const float roll = atan2(sinrCosp, cosrCosp);

    float pitch = 0.0f;
    const float sinp = 2.0f * (data_[3] * data_[1] - data_[2] * data_[0]);
    if (fabs(sinp) >= 1.0f) {
      pitch = copysign(XE_MATH_HALF_PI, sinp);
    } else {
      pitch = asin(sinp);
    }
    const float sinyCosp = 2.0f * (data_[3] * data_[2] + data_[0] * data_[1]);
    const float cosyCosp = 1.0f - 2.0f * (data_[1] * data_[1] + data_[2] * data_[2]);
    const float yaw = atan2(sinyCosp, cosyCosp);

    return vec3(TO_DEG(-roll), TO_DEG(-pitch), TO_DEG(-yaw));
  }

  quat quat::lookRotation(const vec3 &forward, const vec3 &up) {
    const vec3 vector = forward.normalize();
    const vec3 vector2 = up.cross(vector).normalize();
    const vec3 vec3 = vector.cross(vector2);
    const float m00 = vector2[0];
    const float m01 = vector2[1];
    const float m02 = vector2[2];
    const float m10 = vec3[0];
    const float m11 = vec3[1];
    const float m12 = vec3[2];
    const float m20 = vector[0];
    const float m21 = vector[1];
    const float m22 = vector[2];

    const float num8 = (m00 + m11) + m22;
    if (num8 > 0.0f) {
      const float num = sqrtf(num8 + 1.0f);
      const float num1 = 0.5f / num;
      return quat((m12 - m21) * num1,
                  (m20 - m02) * num1,
                  (m01 - m10) * num1,
                  num * 0.5f);
    }
    if ((m00 >= m11) && (m00 >= m22)) {
      const float num7 = sqrtf(((1.0f + m00) - m11) - m22);
      const float num4 = 0.5f / num7;
      return quat(0.5f * num7,
                  (m01 + m10) * num4,
                  (m02 + m20) * num4,
                  (m12 - m21) * num4);
    }
    if (m11 > m22) {
      const float num6 = sqrtf(((1.0f + m11) - m00) - m22);
      const float num3 = 0.5f / num6;
      return quat((m10 + m01) * num3,
                  0.5f * num6,
                  (m21 + m12) * num3,
                  (m20 - m02) * num3);
    }
    const float num5 = sqrtf(((1.0f + m22) - m00) - m11);
    const float num2 = 0.5f / num5;
    return quat((m20 + m02) * num2,
                (m21 + m12) * num2,
                0.5f * num5,
                (m01 - m10) * num2);
  }

}
