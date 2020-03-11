//
// Created by FLXR on 6/14/2019.
//

#ifndef XE_VEC3_HPP
#define XE_VEC3_HPP

#include <xe/common.hpp>
#include <xe/math/vecmath.hpp>
#include <xe/math/vec2.hpp>

namespace xe {

  class vec4;

  class XE_API vec3 {
  public:
    vec3(float x, float y, float z);
    explicit vec3();
    explicit vec3(float val);
    explicit vec3(const vec4 &vec);
    explicit vec3(const Vector &vecIn);

    template<typename T>
    explicit vec3(const tvec2<T> &vec) :
        data_(Vector::make(static_cast<float>(vec.x), static_cast<float>(vec.y), 0.0f, 0.0f)) { }

    void set(float x, float y, float z);
    void set(uint32_t index, float val);

    vec3 normalize(float errorMargin = 1.e-8f) const;
    bool isNormalized(float errorMargin = 1.e-4f) const;

    float max() const;
    float min() const;
    float absMax() const;
    float absMin() const;

    vec3 abs() const;
    vec3 min(const vec3 &other) const;
    vec3 max(const vec3 &other) const;

    void dirAndLength(vec3 &dir, float &length) const;
    vec3 project() const;
    vec3 reciprocal() const;

    vec3 rotate(const vec3 &axis, float angle) const;

    vec3 reflect(const vec3 &normal) const;
    vec3 refract(const vec3 &normal, float indexOfRefraction) const;

    float dot(const vec3 &other) const { return data_.dot3(other.data_)[0]; }
    vec3 dotToVector(const vec3 &other) const { return vec3(data_.dot3(other.data_)); }

    vec3 cross(const vec3 &other) const { return vec3(data_.cross3(other.data_)); }

    float dist(const vec3 &other) const { return sqrtf(distSquared(other)); }
    float distSquared(const vec3 &other) const;

    float length() const { return sqrtf(lengthSquared()); }
    float lengthSquared() const { return data_.dot3(data_)[0]; }

    vec3 toDeg() const;
    vec3 toRad() const;

    bool equals(const vec3 &other, float errorMargin = 1.e-4f) const;
    bool equals(float val, float errorMargin = 1.e-4f) const;

    Vector toVector() const { return data_; }
    Vector toVector(float w) const { return data_.select(VectorConstants::MaskW(), Vector::load1f(w)); }

    static const vec3 &unitX();
    static const vec3 &unitY();
    static const vec3 &unitZ();
    static const vec3 &unitXN();
    static const vec3 &unitYN();
    static const vec3 &unitZN();
    static const vec3 &zero();

    float operator[](uint32_t index) const { return data_[index]; }

    template<typename OStream>
    friend OStream &operator<<(OStream &os, const vec3 &v) {
      return os << "vec3(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
    }

    friend const Node &operator>>(const Node &node, vec3 &vector) {
      float x, y, z;
      node["x"].get(x);
      node["y"].get(y);
      node["z"].get(z);
      vector.set(x, y, z);
      return node;
    }

    friend Node &operator<<(Node &node, const vec3 &vector) {
      node["x"].set(vector[0]);
      node["y"].set(vector[1]);
      node["z"].set(vector[2]);
      return node;
    }

    vec3 operator+(const vec3 &other) const;
    vec3 operator-(const vec3 &other) const;
    vec3 operator*(const vec3 &other) const;
    vec3 operator/(const vec3 &other) const;

    vec3 operator+(float amt) const;
    vec3 operator-(float amt) const;
    vec3 operator*(float amt) const;
    vec3 operator/(float amt) const;

    vec3 operator-() const;
    vec3 operator+=(const vec3 &other);
    vec3 operator-=(const vec3 &other);
    vec3 operator*=(const vec3 &other);
    vec3 operator/=(const vec3 &other);

    vec3 operator+=(float val);
    vec3 operator-=(float val);
    vec3 operator*=(float val);
    vec3 operator/=(float val);

    bool operator==(const vec3 &other) const;
    bool operator!=(const vec3 &other) const;

  private:
    Vector data_;
  };

  inline vec3 vec3::operator+(const vec3 &other) const {
    return vec3(data_ + other.data_);
  }

  inline vec3 vec3::operator-(const vec3 &other) const {
    return vec3(data_ - other.data_);
  }

  inline vec3 vec3::operator*(const vec3 &other) const {
    return vec3(data_ * other.data_);
  }

  inline vec3 vec3::operator/(const vec3 &other) const {
    return vec3(data_ / other.data_);
  }

  inline vec3 vec3::operator+(float amt) const {
    return vec3(data_ + Vector::load1f(amt));
  }

  inline vec3 vec3::operator-(float amt) const {
    return vec3(data_ - Vector::load1f(amt));
  }

  inline vec3 vec3::operator*(float amt) const {
    return vec3(data_ * Vector::load1f(amt));
  }

  inline vec3 vec3::operator/(float amt) const {
    return vec3(data_ * Vector::load1f(math::reciprocal(amt)));
  }

  inline vec3 vec3::operator-() const {
    return vec3(-data_);
  }

  inline vec3 vec3::operator+=(const vec3 &other) {
    data_ = data_ + other.data_;
    return *this;
  }

  inline vec3 vec3::operator-=(const vec3 &other) {
    data_ = data_ - other.data_;
    return *this;
  }

  inline vec3 vec3::operator*=(const vec3 &other) {
    data_ = data_ * other.data_;
    return *this;
  }

  inline vec3 vec3::operator/=(const vec3 &other) {
    data_ = data_ / other.data_;
    return *this;
  }

  inline vec3 vec3::operator+=(float val) {
    data_ = data_ + Vector::load1f(val);
    return *this;
  }

  inline vec3 vec3::operator-=(float val) {
    data_ = data_ - Vector::load1f(val);
    return *this;
  }

  inline vec3 vec3::operator*=(float val) {
    data_ = data_ * Vector::load1f(val);
    return *this;
  }

  inline vec3 vec3::operator/=(float val) {
    data_ = data_ * Vector::load1f(math::reciprocal(val));
    return *this;
  }

  inline bool vec3::operator==(const vec3 &other) const {
    return (data_ != other.data_).isZero3f();
  }

  inline bool vec3::operator!=(const vec3 &other) const {
    return !(*this == other);
  }

}

#endif //XE_VEC3_HPP
