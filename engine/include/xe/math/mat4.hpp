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
    Vector removeScale();

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

    Vector operator[](uint32_t index) const { return data_[index]; }

    template<typename OStream>
    friend OStream &operator<<(OStream &os, const mat4 &q) {
      return os << "\nmat4({" << q[0][0] << ", " << q[0][1] << ", " << q[0][2] << ", " << q[0][3] << "}\n"
                << "     {" << q[1][0] << ", " << q[1][1] << ", " << q[1][2] << ", " << q[1][3] << "}\n"
                << "     {" << q[2][0] << ", " << q[2][1] << ", " << q[2][2] << ", " << q[2][3] << "}\n"
                << "     {" << q[3][0] << ", " << q[3][1] << ", " << q[3][2] << ", " << q[3][3] << "})";
    }

    friend const Node &operator>>(const Node &node, mat4 &mat) {
      float x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3;
      node["m_00"].get(x0);
      node["m_01"].get(y0);
      node["m_02"].get(z0);
      node["m_03"].get(w0);

      node["m_10"].get(x1);
      node["m_11"].get(y1);
      node["m_12"].get(z1);
      node["m_13"].get(w1);

      node["m_20"].get(x2);
      node["m_21"].get(y2);
      node["m_22"].get(z2);
      node["m_23"].get(w2);

      node["m_30"].get(x3);
      node["m_31"].get(y3);
      node["m_32"].get(z3);
      node["m_33"].get(w3);

      mat = mat4(Vector::make(x0, y0, z0, w0), Vector::make(x1, y1, z1, w1), Vector::make(x2, y2, z2, w2), Vector::make(x3, y3, z3, w3));
      return node;
    }

    friend Node &operator<<(Node &node, const mat4 &mat) {
      node["m_00"].set(mat[0][0]);
      node["m_01"].set(mat[0][1]);
      node["m_02"].set(mat[0][2]);
      node["m_03"].set(mat[0][3]);

      node["m_10"].set(mat[1][0]);
      node["m_11"].set(mat[1][1]);
      node["m_12"].set(mat[1][2]);
      node["m_13"].set(mat[1][3]);

      node["m_20"].set(mat[2][0]);
      node["m_21"].set(mat[2][1]);
      node["m_22"].set(mat[2][2]);
      node["m_23"].set(mat[2][3]);

      node["m_30"].set(mat[3][0]);
      node["m_31"].set(mat[3][1]);
      node["m_32"].set(mat[3][2]);
      node["m_33"].set(mat[3][3]);
      return node;
    }

    mat4 operator+(const mat4 &other) const;
    mat4 &operator+=(const mat4 &other);
    mat4 operator*(const mat4 &other) const;
    mat4 &operator*=(const mat4 &other);
    mat4 operator*(float amt) const;
    mat4 &operator*=(float amt);

    bool operator==(const mat4 &other) const;
    bool operator!=(const mat4 &other) const;

    vec3 operator*(const vec3 &v) const;

  private:
    Vector data_[4];
  };

  inline mat4 mat4::operator+(const mat4 &other) const {
    mat4 result;
    for (uint32_t i = 0; i < 4; i++) {
      result.data_[i] = data_[i] + other.data_[i];
    }
    return result;
  }

  inline mat4 &mat4::operator+=(const mat4 &other) {
    for (uint32_t i = 0; i < 4; i++) {
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
    for (uint32_t i = 0; i < 4; i++) {
      result.data_[i] = data_[i] * vecAmt;
    }
    return result;
  }

  inline mat4 &mat4::operator*=(float amt) {
    const Vector vecAmt = Vector::load1f(amt);
    for (uint32_t i = 0; i < 4; i++) {
      data_[i] = data_[i] * vecAmt;
    }
    return *this;
  }

  inline bool mat4::operator==(const mat4 &other) const {
    for (uint32_t i = 0; i < 4; i++) {
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
