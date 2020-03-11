//
// Created by FLXR on 6/21/2019.
//

#ifndef XE_QUAT_HPP
#define XE_QUAT_HPP


#include <xe/common.hpp>
#include <xe/math/vecmath.hpp>
#include <xe/serialization/node.hpp>

namespace xe {

  class vec3;

  class XE_API quat {
  public:
    explicit quat();
    explicit quat(const Vector &quat);
    explicit quat(const vec3 &eulerAngle);
    quat(float x, float y, float z, float w);
    quat(const vec3 &axis, float angleDeg);

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

    vec3 toEulerAngles() const;

    Vector toVector() const { return data_; }

    static quat lookRotation(const vec3 &forward, const vec3 &up);

    float operator[](uint32_t index) const { return data_[index]; }

    template<typename OStream>
    friend OStream &operator<<(OStream &os, const quat &q) {
      return os << "quat(" << q[0] << ", " << q[1] << ", " << q[2] << ", " << q[3] << ")";
    }

    friend const Node &operator>>(const Node &node, quat &q) {
      float x, y, z, w;
      node["x"].get(x);
      node["y"].get(y);
      node["z"].get(z);
      node["w"].get(w);
      q = quat(x, y, z, w);
      return node;
    }

    friend Node &operator<<(Node &node, const quat &q) {
      node["x"].set(q[0]);
      node["y"].set(q[1]);
      node["z"].set(q[2]);
      node["w"].set(q[3]);
      return node;
    }

    quat operator+(const quat &other) const;
    quat operator+=(const quat &other);
    quat operator-(const quat &other) const;
    quat operator-=(const quat &other);
    quat operator*(const quat &other) const;
    quat operator*=(const quat &other);
    quat operator*(float amt) const;
    quat operator*=(float amt);
    quat operator/(float amt) const;
    quat operator/=(float amt);
    vec3 operator*(const vec3 &other) const;

    bool operator==(const quat &other) const;
    bool operator!=(const quat &other) const;

  private:
    Vector data_;
  };

  template<>
  inline quat math::lerp(const quat &val1, const quat &val2, const float &amt) {
    float dotAmt = val1.dot(val2);
    float dir = math::select(dotAmt, 1.0f, -1.0f);
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
    return *this * math::reciprocal(amt);
  }

  inline quat quat::operator/=(float amt) {
    return *this *= math::reciprocal(amt);
  }

  inline bool quat::operator==(const quat &other) const {
    return (data_ != other.data_).isZero4f();
  }

  inline bool quat::operator!=(const quat &other) const {
    return (data_ == other.data_).isZero4f();
  }

}


#endif //XE_QUAT_HPP
