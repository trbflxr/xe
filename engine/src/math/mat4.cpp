//
// Created by FLXR on 6/21/2019.
//

#include <xe/math/mat4.hpp>

namespace xe {

  mat4::mat4() {
    *this = identity();
  }

  mat4::mat4(const Vector &vecX, const Vector &vecY, const Vector &vecZ, const Vector &vecOffset) {
    data_[0] = vecX;
    data_[1] = vecY;
    data_[2] = vecZ;
    data_[3] = vecOffset;
  }

  mat4 mat4::transpose() const {
    mat4 result;
    float m0[4];
    float m1[4];
    float m2[4];
    float m3[4];
    data_[0].store4f(m0);
    data_[1].store4f(m1);
    data_[2].store4f(m2);
    data_[3].store4f(m3);

    for (uint i = 0; i < 4; i++) {
      result.data_[i] = Vector::make(m0[i], m1[i], m2[i], m3[i]);
    }

    return result;
  }

  float mat4::determinant4x4() const {
    return Vector::matrixDeterminant4x4(nullptr, nullptr, data_);
  }

  float mat4::determinant3x3() const {
    return Vector::matrixDeterminant3x3Vector(data_);
  }

  mat4 mat4::inverse() const {
    mat4 result;
    Vector::matrixInverse(&result, this);
    return result;
  }

  mat4 mat4::applyScale(const Vector &scale) {
    for (uint i = 0; i < 4; i++) {
      data_[i] = data_[i] * scale;
    }
    return *this;
  }

  Vector mat4::removeScale(float errorMargin) {
    Vector invScale = VectorConstants::Zero();
    for (uint i = 0; i < 4; i++) {
      invScale = invScale + data_[i] * data_[i];
    }
    invScale = invScale.rsqrt().select(VectorConstants::MaskW(), VectorConstants::One());
    for (uint i = 0; i < 4; i++) {
      data_[i] = data_[i] * invScale;
    }
    return invScale.reciprocal();
  }

  Vector mat4::getScale() const {
    Vector invScale = VectorConstants::Zero();
    for (uint i = 0; i < 4; i++) {
      invScale = invScale + data_[i] * data_[i];
    }
    invScale = invScale.rsqrt().select(VectorConstants::MaskW(), VectorConstants::One());
    return invScale.reciprocal();
  }

  quat mat4::getRotation() const {
    float result[4];
    float m[4][4];

    mat4 temp(*this);
    temp.removeScale();

    for (uint i = 0; i < 4; i++) {
      temp.data_[i].store4f(m[i]);
    }
    const float trace = m[0][0] + m[1][1] + m[2][2];
    if (trace > 0) {
      const float s = 0.5f * math::rsqrt(trace + 1.0f);
      result[3] = 0.25f / s;
      result[0] = (m[1][2] - m[2][1]) * s;
      result[1] = (m[2][0] - m[0][2]) * s;
      result[2] = (m[0][1] - m[1][0]) * s;
    } else {
      if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
        float s = 2.0f * sqrtf(1.0f + m[0][0] - m[1][1] - m[2][2]);
        result[3] = (m[1][2] - m[2][1]) / s;
        result[0] = 0.25f * s;
        result[1] = (m[1][0] + m[0][1]) / s;
        result[2] = (m[2][0] + m[0][2]) / s;
      } else if (m[1][1] > m[2][2]) {
        const float s = 2.0f * sqrtf(1.0f + m[1][1] - m[0][0] - m[2][2]);
        result[3] = (m[2][0] - m[0][2]) / s;
        result[0] = (m[1][0] + m[0][1]) / s;
        result[1] = 0.25f * s;
        result[2] = (m[1][2] + m[2][1]) / s;
      } else {
        const float s = 2.0f * sqrtf(1.0f + m[2][2] - m[0][0] - m[1][1]);
        result[3] = (m[0][1] - m[1][0]) / s;
        result[0] = (m[2][0] + m[0][2]) / s;
        result[1] = (m[1][2] + m[2][1]) / s;
        result[2] = 0.25f * s;
      }
    }
    return quat(result[0], result[1], result[2], -result[3]).normalize();
  }

  Vector mat4::getTranslation() const {
    return Vector::make(data_[0][3], data_[1][3], data_[2][3], data_[3][3]);
  }

  Vector mat4::transform(const Vector &vector) const {
    return vector.transform(this);
  }

  bool mat4::equals(const mat4 &other, float errorMargin) const {
    for (uint i = 0; i < 4; i++) {
      if (!(data_[i].notEquals(other.data_[i], errorMargin)).isZero4f()) {
        return false;
      }
    }
    return true;
  }

  mat4 mat4::identity() {
    return mat4(Vector::make(1.0f, 0.0f, 0.0f, 0.0f),
                Vector::make(0.0f, 1.0f, 0.0f, 0.0f),
                Vector::make(0.0f, 0.0f, 1.0f, 0.0f),
                Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
  }

  mat4 mat4::ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    const float rwidth = math::reciprocal(right - left);
    const float rheight = math::reciprocal(top - bottom);
    const float rdepth = math::reciprocal(farPlane - nearPlane);

    return mat4(Vector::make(2.0f * rwidth, 0.0f, 0.0f, -(right + left) * rwidth),
                Vector::make(0.0f, 2.0f * rheight, 0.0f, -(top + bottom) * rheight),
                Vector::make(0.0f, 0.0f, 2.0f * rdepth, -(farPlane + nearPlane) * rdepth),
                Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
  }

  mat4 mat4::perspective(float fovDeg, float aspect, float nearZ, float farZ) {
    const float tanHalfFov = tanf(TO_RAD(fovDeg / 2.0f));

    return mat4(Vector::make(1.0f / (aspect * tanHalfFov), 0.0f, 0.0f, 0.0f),
                Vector::make(0.0f, 1.0f / tanHalfFov, 0.0f, 0.0f),
                Vector::make(0.0f, 0.0f, -(farZ + nearZ) / (farZ - nearZ), -1.0f),
                Vector::make(0.0f, 0.0f, -2.0f * farZ * nearZ / (farZ - nearZ), 0.0f));
  }


  mat4 mat4::translation(const vec3 &amt) {
    return mat4(Vector::make(1.0f, 0.0f, 0.0f, amt[0]),
                Vector::make(0.0f, 1.0f, 0.0f, amt[1]),
                Vector::make(0.0f, 0.0f, 1.0f, amt[2]),
                Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
  }

  mat4 mat4::rotation(float angleDeg, const vec3 &axis) {
    mat4 m = mat4::identity();

    const float r = TO_RAD(angleDeg);
    const float c = cosf(r);
    const float s = sinf(r);
    const float omc = 1.0f - c;

    const float x = axis[0];
    const float y = axis[1];
    const float z = axis[2];

    m.data_[0] = Vector::make(x * x * omc + c, y * x * omc + z * s, x * z * omc - y * s, 0.0f);
    m.data_[1] = Vector::make(x * y * omc - z * s, y * y * omc + c, y * z * omc + x * s, 0.0f);
    m.data_[2] = Vector::make(x * z * omc + y * s, y * z * omc - x * s, z * z * omc + c, 0.0f);

    return m;
  }

  mat4 mat4::rotation(const quat &q) {
    mat4 m = mat4::identity();

    const float qx = q[0];
    const float qy = q[1];
    const float qz = q[2];
    const float qw = q[3];
    const float qx2 = (qx + qx);
    const float qy2 = (qy + qy);
    const float qz2 = (qz + qz);
    const float qxqx2 = (qx * qx2);
    const float qxqy2 = (qx * qy2);
    const float qxqz2 = (qx * qz2);
    const float qxqw2 = (qw * qx2);
    const float qyqy2 = (qy * qy2);
    const float qyqz2 = (qy * qz2);
    const float qyqw2 = (qw * qy2);
    const float qzqz2 = (qz * qz2);
    const float qzqw2 = (qw * qz2);

    m.data_[0] = Vector::make(((1.0f - qyqy2) - qzqz2), (qxqy2 - qzqw2), (qxqz2 + qyqw2), 0.0f);
    m.data_[1] = Vector::make((qxqy2 + qzqw2), ((1.0f - qxqx2) - qzqz2), (qyqz2 - qxqw2), 0.0f);
    m.data_[2] = Vector::make((qxqz2 - qyqw2), (qyqz2 + qxqw2), ((1.0f - qxqx2) - qyqy2), 0.0f);

    return m;
  }

  mat4 mat4::scale(const vec3 &amt) {
    return mat4(Vector::make(amt[0], 0.0f, 0.0f, 0.0f),
                Vector::make(0.0f, amt[1], 0.0f, 0.0f),
                Vector::make(0.0f, 0.0f, amt[2], 0.0f),
                Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
  }

  mat4 mat4::scale(float amt) {
    return scale(vec3(amt));
  }

  mat4 mat4::transformation(const vec3 &translation, const quat &rotation, const vec3 &scale) {
    mat4 result;
    Vector::createTransformMatrix(&result, translation.toVector(), rotation.toVector(), scale.toVector());
    return result;
  }

}
