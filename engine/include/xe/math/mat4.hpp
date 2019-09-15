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

    Vector operator[](uint index) const { return data_[index]; }

    template<typename OStream>
    inline friend OStream &operator<<(OStream &os, const mat4 &q) {
      return os << "\nmat4({" << q[0][0] << ", " << q[0][1] << ", " << q[0][2] << ", " << q[0][3] << "}\n"
                << "     {" << q[1][0] << ", " << q[1][1] << ", " << q[1][2] << ", " << q[1][3] << "}\n"
                << "     {" << q[2][0] << ", " << q[2][1] << ", " << q[2][2] << ", " << q[2][3] << "}\n"
                << "     {" << q[3][0] << ", " << q[3][1] << ", " << q[3][2] << ", " << q[3][3] << "})";
    }

    inline mat4 operator+(const mat4 &other) const;
    inline mat4 &operator+=(const mat4 &other);
    inline mat4 operator*(const mat4 &other) const;
    inline mat4 &operator*=(const mat4 &other);
    inline mat4 operator*(float amt) const;
    inline mat4 &operator*=(float amt);

    inline bool operator==(const mat4 &other) const;
    inline bool operator!=(const mat4 &other) const;

    inline vec3 operator*(const vec3 &v) const;

  private:
    Vector data_[4];
  };

  inline mat4 mat4::operator+(const mat4 &other) const {
    mat4 result;
    for (uint i = 0; i < 4; i++) {
      result.data_[i] = data_[i] + other.data_[i];
    }
    return result;
  }

  inline mat4 &mat4::operator+=(const mat4 &other) {
    for (uint i = 0; i < 4; i++) {
      data_[i] = data_[i] + other.data_[i];
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
      result.data_[i] = data_[i] * vecAmt;
    }
    return result;
  }

  inline mat4 &mat4::operator*=(float amt) {
    const Vector vecAmt = Vector::load1f(amt);
    for (uint i = 0; i < 4; i++) {
      data_[i] = data_[i] * vecAmt;
    }
    return *this;
  }

  inline bool mat4::operator==(const mat4 &other) const {
    for (uint i = 0; i < 4; i++) {
      if (!(data_[i] != other.data_[i]).isZero4f()) {
        return false;
      }
    }
    return true;
  }

  inline bool mat4::operator!=(const mat4 &other) const {
    return !(*this == other);
  }

  inline vec3 mat4::operator*(const vec3 &v) const {
    return vec3(data_[0][0] * v[0] + data_[0][1] * v[1] + data_[0][2] * v[2] + data_[0][3] * 0.0f,
                data_[1][0] * v[0] + data_[1][1] * v[1] + data_[1][2] * v[2] + data_[1][3] * 0.0f,
                data_[2][0] * v[0] + data_[2][1] * v[1] + data_[2][2] * v[2] + data_[2][3] * 0.0f);
  }

}

#endif //XE_MAT4_HPP
