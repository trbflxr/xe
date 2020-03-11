//
// Created by FLXR on 6/15/2019.
//

#ifndef XE_VEC4_HPP
#define XE_VEC4_HPP

#include <xe/common.hpp>
#include <xe/math/vecmath.hpp>
#include <xe/math/vec2.hpp>

namespace xe {

  class vec3;

  class XE_API vec4 {
  public:
    vec4(float x, float y, float z, float w);
    explicit vec4(const vec3 &vec);
    explicit vec4();
    explicit vec4(float val);
    explicit vec4(const Vector &vecIn);

    template<typename T>
    explicit vec4(const tvec2<T> &vec) :
        data_(Vector::make(static_cast<float>(vec.x), static_cast<float>(vec.y), 0.0f, 0.0f)) { }

    void set(float x, float y, float z, float w);
    void set(uint32_t index, float val);

    float dot(const vec4 &other) const { return data_.dot4(other.data_)[0]; }

    Vector toVector() const { return data_; }

    bool equals(const vec4 &other, float errorMargin = 1.e-4f) const;
    bool equals(float val, float errorMargin = 1.e-4f) const;

    float operator[](uint32_t index) const { return data_[index]; }

    template<typename OStream>
    friend OStream &operator<<(OStream &os, const vec4 &v) {
      return os << "vec4(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
    }

    friend const Node &operator>>(const Node &node, vec4 &vector) {
      float x, y, z, w;
      node["x"].get(x);
      node["y"].get(y);
      node["z"].get(z);
      node["w"].get(w);
      vector.set(x, y, z, w);
      return node;
    }

    friend Node &operator<<(Node &node, const vec4 &vector) {
      node["x"].set(vector[0]);
      node["y"].set(vector[1]);
      node["z"].set(vector[2]);
      node["w"].set(vector[3]);
      return node;
    }

    vec4 operator+(const vec4 &other) const;
    vec4 operator-(const vec4 &other) const;
    vec4 operator*(const vec4 &other) const;
    vec4 operator/(const vec4 &other) const;

    vec4 operator+(float amt) const;
    vec4 operator-(float amt) const;
    vec4 operator*(float amt) const;
    vec4 operator/(float amt) const;

    vec4 operator-() const;
    vec4 operator+=(const vec4 &other);
    vec4 operator-=(const vec4 &other);
    vec4 operator*=(const vec4 &other);
    vec4 operator/=(const vec4 &other);

    vec4 operator+=(float val);
    vec4 operator-=(float val);
    vec4 operator*=(float val);
    vec4 operator/=(float val);

    bool operator==(const vec4 &other) const;
    bool operator!=(const vec4 &other) const;

  private:
    Vector data_;
  };


  inline vec4 vec4::operator+(const vec4 &other) const {
    return vec4(data_ + other.data_);
  }

  inline vec4 vec4::operator-(const vec4 &other) const {
    return vec4(data_ - other.data_);
  }

  inline vec4 vec4::operator*(const vec4 &other) const {
    return vec4(data_ * other.data_);
  }

  inline vec4 vec4::operator/(const vec4 &other) const {
    return vec4(data_ / other.data_);
  }

  inline vec4 vec4::operator+(float amt) const {
    return vec4(data_ + Vector::load1f(amt));
  }

  inline vec4 vec4::operator-(float amt) const {
    return vec4(data_ - Vector::load1f(amt));
  }

  inline vec4 vec4::operator*(float amt) const {
    return vec4(data_ * Vector::load1f(amt));
  }

  inline vec4 vec4::operator/(float amt) const {
    return vec4(data_ * Vector::load1f(math::reciprocal(amt)));
  }

  inline vec4 vec4::operator-() const {
    return vec4(-data_);
  }

  inline vec4 vec4::operator+=(const vec4 &other) {
    data_ = data_ + other.data_;
    return *this;
  }

  inline vec4 vec4::operator-=(const vec4 &other) {
    data_ = data_ - other.data_;
    return *this;
  }

  inline vec4 vec4::operator*=(const vec4 &other) {
    data_ = data_ * other.data_;
    return *this;
  }

  inline vec4 vec4::operator/=(const vec4 &other) {
    data_ = data_ / other.data_;
    return *this;
  }

  inline vec4 vec4::operator+=(float val) {
    data_ = data_ + Vector::load1f(val);
    return *this;
  }

  inline vec4 vec4::operator-=(float val) {
    data_ = data_ - Vector::load1f(val);
    return *this;
  }

  inline vec4 vec4::operator*=(float val) {
    data_ = data_ * Vector::load1f(val);
    return *this;
  }

  inline vec4 vec4::operator/=(float val) {
    data_ = data_ * Vector::load1f(math::reciprocal(val));
    return *this;
  }

  inline bool vec4::operator==(const vec4 &other) const {
    return (data_ != other.data_).isZero4f();
  }

  inline bool vec4::operator!=(const vec4 &other) const {
    return !(*this == other);
  }

}

#endif //XE_VEC4_HPP
