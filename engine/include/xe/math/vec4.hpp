//
// Created by FLXR on 6/15/2019.
//

#ifndef XE_VEC4_HPP
#define XE_VEC4_HPP


#include <xe/common.hpp>
#include <xe/math/vecmath.hpp>

namespace xe {

  class vec2;
  class vec3;

  class XE_API vec4 {
  public:
    vec4(float x, float y, float z, float w);
    explicit vec4(const vec2 &vec);
    explicit vec4(const vec3 &vec);
    explicit vec4();
    explicit vec4(float val);
    explicit vec4(const Vector &vecIn);

    void set(float x, float y, float z, float w);
    void set(uint index, float val);

    inline float dot(const vec4 &other) const { return data_.dot4(other.data_)[0]; }

    inline Vector toVector() const { return data_; }

    bool equals(const vec4 &other, float errorMargin = 1.e-4f) const;
    bool equals(float val, float errorMargin = 1.e-4f) const;

    inline float operator[](uint index) const { return data_[index]; }

    inline vec4 operator+(const vec4 &other) const;
    inline vec4 operator-(const vec4 &other) const;
    inline vec4 operator*(const vec4 &other) const;
    inline vec4 operator/(const vec4 &other) const;

    inline vec4 operator+(float amt) const;
    inline vec4 operator-(float amt) const;
    inline vec4 operator*(float amt) const;
    inline vec4 operator/(float amt) const;

    inline vec4 operator-() const;
    inline vec4 operator+=(const vec4 &other);
    inline vec4 operator-=(const vec4 &other);
    inline vec4 operator*=(const vec4 &other);
    inline vec4 operator/=(const vec4 &other);

    inline vec4 operator+=(float val);
    inline vec4 operator-=(float val);
    inline vec4 operator*=(float val);
    inline vec4 operator/=(float val);

    inline bool operator==(const vec4 &other) const;
    inline bool operator!=(const vec4 &other) const;

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
    return vec4(data_ * Vector::load1f(xe::reciprocal(amt)));
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
    data_ = data_ * Vector::load1f(xe::reciprocal(val));
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
