//
// Created by FLXR on 6/21/2019.
//

#ifndef XE_MAT4_HPP
#define XE_MAT4_HPP


#include <xe/common.hpp>
#include <xe/math/vecmath.hpp>
#include <xe/math/vec3.hpp>
#include <xe/math/quat.hpp>

namespace xe {

  class XE_API mat4 {
  public:
    explicit mat4();
    mat4(const Vector &vecX, const Vector &vecY, const Vector &vecZ, const Vector &vecOffset);

    mat4 transpose() const;
    float determinant4x4() const;
    float determinant3x3() const;
    mat4 inverse() const;

    mat4 applyScale(const Vector &scale);
    Vector removeScale(float errorMargin = 1.e-8f);

    Vector getScale() const;
    quat getRotation() const;
    Vector getTranslation() const;

    Vector transform(const Vector &vector) const;

    bool equals(const mat4 &other, float errorMargin = 1.e-4f) const;

    static mat4 identity();

    static mat4 ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    static mat4 perspective(float fovDeg, float aspect, float nearZ, float farZ);

    static mat4 translation(const vec3 &amt);

    static mat4 rotation(float angleDeg, const vec3 &axis);
    static mat4 rotation(const quat &q);

    static mat4 scale(const vec3 &amt);
    static mat4 scale(float amt);

    static mat4 transformation(const vec3 &translation, const quat &rotation, const vec3 &scale = vec3(1.0f));

    Vector operator[](uint index) const { return _data[index]; }

    inline mat4 operator+(const mat4 &other) const;
    inline mat4 &operator+=(const mat4 &other);
    inline mat4 operator*(const mat4 &other) const;
    inline mat4 &operator*=(const mat4 &other);
    inline mat4 operator*(float amt) const;
    inline mat4 &operator*=(float amt);

    inline bool operator==(const mat4 &other) const;
    inline bool operator!=(const mat4 &other) const;

  private:
    Vector _data[4];
  };

  inline mat4 mat4::operator+(const mat4 &other) const {
    mat4 result;
    for (uint i = 0; i < 4; i++) {
      result._data[i] = _data[i] + other._data[i];
    }
    return result;
  }

  inline mat4 &mat4::operator+=(const mat4 &other) {
    for (uint i = 0; i < 4; i++) {
      _data[i] = _data[i] + other._data[i];
    }
    return *this;
  }

  inline mat4 mat4::operator*(const mat4 &other) const {
    mat4 result;
    Vector::matrixMul(&result, this, &other);
    return result;
  }

  inline mat4 &mat4::operator*=(const mat4 &other) {
    Vector::matrixMul(this, this, &other);
    return *this;
  }

  inline mat4 mat4::operator*(float amt) const {
    mat4 result;
    const Vector vecAmt = Vector::load1f(amt);
    for (uint i = 0; i < 4; i++) {
      result._data[i] = _data[i] * vecAmt;
    }
    return result;
  }

  inline mat4 &mat4::operator*=(float amt) {
    const Vector vecAmt = Vector::load1f(amt);
    for (uint i = 0; i < 4; i++) {
      _data[i] = _data[i] * vecAmt;
    }
    return *this;
  }

  inline bool mat4::operator==(const mat4 &other) const {
    for (uint i = 0; i < 4; i++) {
      if (!(_data[i] != other._data[i]).isZero4f()) {
        return false;
      }
    }
    return true;
  }

  inline bool mat4::operator!=(const mat4 &other) const {
    return !(*this == other);
  }


}

#endif //XE_MAT4_HPP
