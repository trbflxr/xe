//
// Created by FLXR on 6/14/2019.
//

#ifndef XE_VEC2_HPP
#define XE_VEC2_HPP


#include <xe/common.hpp>
#include <xe/math/vecmath.hpp>

namespace xe {

  class vec3;
  class vec4;

  class XE_API vec2 {
  public:
    float x;
    float y;

    vec2(float x, float y);
    explicit vec2();
    explicit vec2(float value);
    explicit vec2(const vec3 &vec);
    explicit vec2(const vec4 &vec);

    vec2 normalize(float errorMargin = 1.e-8f) const;
    bool isNormalized(float errorMargin = 1.e-4f) const;

    float max() const;
    float min() const;
    float absMax() const;
    float absMin() const;

    vec2 abs() const;
    vec2 min(const vec2 &other) const;
    vec2 max(const vec2 &other) const;

    void dirAndLength(vec2 &dir, float &length, float errorMargin = 1.e-8f) const;
    vec2 reciprocal() const;

    vec2 rotate(float angle) const;

    vec2 reflect(const vec2 &normal) const;
    vec2 refract(const vec2 &normal, float indexOfRefraction) const;

    inline float dot(const vec2 &other) const { return x * other.x + y * other.y; }
    inline vec2 dotToVector(const vec2 &other) const { return vec2(dot(other)); }

    inline float cross(const vec2 &other) const { return x * other.y - y * other.x; }

    inline float dist(const vec2 &other) const { return sqrtf(distSquared(other)); }
    inline float distSquared(const vec2 &other) const { return (other - *this).lengthSquared(); }

    inline float length() const { return sqrtf(lengthSquared()); }
    inline float lengthSquared() const { return dot(*this); }

    vec2 toDeg() const;
    vec2 toRad() const;

    bool equals(const vec2 &other, float errorMargin = 1.e-4f) const;
    bool equals(float val, float errorMargin = 1.e-4f) const;

    inline Vector toVector() const { return toVector(0.0f, 0.0f); }
    inline Vector toVector(float z, float w) const { return toVector(vec2(z, w)); }
    inline Vector toVector(vec2 other) const { return Vector::make(x, y, other.x, other.y); }

    inline vec2 operator+(const vec2 &other) const;
    inline vec2 operator-(const vec2 &other) const;
    inline vec2 operator*(const vec2 &other) const;
    inline vec2 operator/(const vec2 &other) const;

    inline vec2 operator+(float amt) const;
    inline vec2 operator-(float amt) const;
    inline vec2 operator*(float amt) const;
    inline vec2 operator/(float amt) const;

    inline vec2 operator-() const;
    inline vec2 operator+=(const vec2 &other);
    inline vec2 operator-=(const vec2 &other);
    inline vec2 operator*=(const vec2 &other);
    inline vec2 operator/=(const vec2 &other);

    inline vec2 operator+=(float val);
    inline vec2 operator-=(float val);
    inline vec2 operator*=(float val);
    inline vec2 operator/=(float val);

    inline bool operator==(const vec2 &other) const;
    inline bool operator!=(const vec2 &other) const;
  };

  inline vec2 vec2::operator+(const vec2 &other) const {
    return vec2(x + other.x, y + other.y);
  }

  inline vec2 vec2::operator-(const vec2 &other) const {
    return vec2(x - other.x, y - other.y);
  }

  inline vec2 vec2::operator*(const vec2 &other) const {
    return vec2(x * other.x, y * other.y);
  }

  inline vec2 vec2::operator/(const vec2 &other) const {
    return vec2(x / other.x, y / other.y);
  }

  inline vec2 vec2::operator+(float amt) const {
    return vec2(x + amt, y + amt);
  }

  inline vec2 vec2::operator-(float amt) const {
    return vec2(x - amt, y - amt);
  }

  inline vec2 vec2::operator*(float amt) const {
    return vec2(x * amt, y * amt);
  }

  inline vec2 vec2::operator/(float amt) const {
    return vec2(x / amt, y / amt);
  }

  inline vec2 vec2::operator-() const {
    return vec2(-x, -y);
  }

  inline vec2 vec2::operator+=(const vec2 &other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  inline vec2 vec2::operator-=(const vec2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  inline vec2 vec2::operator*=(const vec2 &other) {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  inline vec2 vec2::operator/=(const vec2 &other) {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  inline vec2 vec2::operator+=(float val) {
    x += val;
    y += val;
    return *this;
  }

  inline vec2 vec2::operator-=(float val) {
    x -= val;
    y -= val;
    return *this;
  }

  inline vec2 vec2::operator*=(float val) {
    x *= val;
    y *= val;
    return *this;
  }

  inline vec2 vec2::operator/=(float val) {
    x /= val;
    y /= val;
    return *this;
  }

  inline bool vec2::operator==(const vec2 &other) const {
    return x == other.x && y == other.y;
  }

  inline bool vec2::operator!=(const vec2 &other) const {
    return x != other.x || y != other.y;
  }

}


#endif //XE_VEC2_HPP
