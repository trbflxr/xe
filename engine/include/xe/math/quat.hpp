//
// Created by FLXR on 6/21/2019.
//

#ifndef XE_QUAT_HPP
#define XE_QUAT_HPP


#include <xe/common.hpp>
#include <xe/math/vecmath.hpp>

namespace xe {

  class vec3;

  class XE_API quat {
  public:
    explicit quat();
    explicit quat(const Vector &quat);
    quat(float x, float y, float z, float w);
    explicit quat(const vec3 &axis, float angleDeg);

    quat normalize(float errorMargin = 1.e-8f) const;
    bool isNormalized(float errorMargin = 1.e-4f) const;

    vec3 rotate(const vec3 &other) const;
    quat slerp(const quat &dest, float amt, float errorMargin = 1.e-4f) const;
    quat conjugate() const;
    quat inverse() const;

    float dot(const quat &other) const;
    float length() const;
    float lengthSquared() const;

    vec3 getAxis() const;
    float getAngle() const;
    void axisAndAngle(vec3 &axis, float &angleDeg) const;

    vec3 getForward() const;
    vec3 getBackward() const;
    vec3 getUp() const;
    vec3 getDown() const;
    vec3 getRight() const;
    vec3 getLeft() const;

    vec3 getAxisX() const;
    vec3 getAxisY() const;
    vec3 getAxisZ() const;

    bool equals(const quat &other, float errorMargin = 1.e-4f) const;

    inline Vector toVector() const { return data_; }

    inline float operator[](uint index) const { return data_[index]; }

    inline quat operator+(const quat &other) const;
    inline quat operator+=(const quat &other);
    inline quat operator-(const quat &other) const;
    inline quat operator-=(const quat &other);
    inline quat operator*(const quat &other) const;
    inline quat operator*=(const quat &other);
    inline quat operator*(float amt) const;
    inline quat operator*=(float amt);
    inline quat operator/(float amt) const;
    inline quat operator/=(float amt);
    vec3 operator*(const vec3 &other) const;

    inline bool operator==(const quat &other) const;
    inline bool operator!=(const quat &other) const;

  private:
    Vector data_;
  };

  template<>
  inline quat lerp(const quat &val1, const quat &val2, const float &amt) {
    float dotAmt = val1.dot(val2);
    float dir = select(dotAmt, 1.0f, -1.0f);
    return (val2 * amt) + val1 * (dir * (1.0f - amt));
  }

  inline quat quat::operator+(const quat &other) const {
    return quat(data_ + other.data_);
  }

  inline quat quat::operator+=(const quat &other) {
    data_ = data_ + other.data_;
    return *this;
  }

  inline quat quat::operator-(const quat &other) const {
    return quat(data_ - other.data_);
  }

  inline quat quat::operator-=(const quat &other) {
    data_ = data_ - other.data_;
    return *this;
  }

  inline quat quat::operator*(const quat &other) const {
    return quat(data_.quatMul(other.data_));
  }

  inline quat quat::operator*=(const quat &other) {
    data_ = data_.quatMul(other.data_);
    return *this;
  }

  inline quat quat::operator*(float amt) const {
    return quat(data_ * Vector::load1f(amt));
  }

  inline quat quat::operator*=(float amt) {
    data_ = data_ * Vector::load1f(amt);
    return *this;
  }

  inline quat quat::operator/(float amt) const {
    return *this * reciprocal(amt);
  }

  inline quat quat::operator/=(float amt) {
    return *this *= reciprocal(amt);
  }

  inline bool quat::operator==(const quat &other) const {
    return (data_ != other.data_).isZero4f();
  }

  inline bool quat::operator!=(const quat &other) const {
    return (data_ == other.data_).isZero4f();
  }

}


#endif //XE_QUAT_HPP
