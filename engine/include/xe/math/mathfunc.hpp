//
// Created by FLXR on 6/14/2019.
//

#ifndef XE_MATHFUNC_HPP
#define XE_MATHFUNC_HPP

#include <cmath>

#define XE_MATH_PI                3.1415926535897932f
#define XE_MATH_TWO_PI            6.28318530717959f
#define XE_MATH_HALF_PI           1.57079632679f
#define XE_MATH_R_PI              0.31830988618f
#define XE_MATH_R_TWO_PI          0.159154943091895f
#define XE_MATH_R_HALF_PI         0.636619772367581f

#define XE_MATH_E                 2.71828182845904523536f
#define XE_MATH_R_LN_2            1.44269504088896f
#define XE_MATH_RAD_TO_DEG        57.2957795130823f
#define XE_MATH_DEG_TO_RAD        0.0174532925199433f

#define TO_RAD(deg)               ((deg) * XE_MATH_PI / 180.0f)
#define TO_DEG(rad)               ((rad) * 180.0f / XE_MATH_PI)

namespace xe::math {

  template<typename T>
  static constexpr T select(const T &cmp, const T &greaterOrEqualToZero, const T &lessZero) {
    return cmp >= (T) (0) ? greaterOrEqualToZero : lessZero;
  }

  static void sincos(float *outSin, float *outCos, float angle) {
    if ((angle < 0.0f) || (angle >= XE_MATH_TWO_PI)) {
      angle -= floorf(angle * XE_MATH_R_TWO_PI) * XE_MATH_TWO_PI;
    }

    angle = XE_MATH_PI - angle;
    float sign = -1.0f;
    if (fabsf(angle) >= XE_MATH_HALF_PI) {
      angle = select(angle, XE_MATH_PI, -XE_MATH_PI) - angle;
      sign = 1.0f;
    }

    const float a2 = angle * angle;
    *outSin = angle * (((((-2.39e-08f * a2 + 2.7526e-06f) * a2 - 1.98409e-04f) *
                         a2 + 8.3333315e-03f) * a2 - 1.666666664e-01f) * a2 + 1.0f);
    *outCos = sign * (((((-2.605e-07f * a2 + 2.47609e-05f) * a2 - 1.3888397e-03f) *
                        a2 + 4.16666418e-02f) * a2 - 4.999999963e-01f) * a2 + 1.0f);
  }

  template<typename T, typename U>
  static T lerp(const T &val1, const T &val2, const U &amt) {
    return (T) (val1 * ((U) (1) - amt) + val2 * amt);
  }

  template<typename T>
  static T reciprocal(T val) {
    return 1 / val;
  }

  template<typename T>
  static T rsqrt(T val) {
    return reciprocal(std::sqrt(val));
  }

  template<typename T>
  static constexpr T min3(const T &val1, const T &val2, const T &val3) {
    return std::min(std::min(val1, val2), val3);
  }

  template<typename T>
  static constexpr T max3(const T &val1, const T &val2, const T &val3) {
    return std::max(std::max(val1, val2), val3);
  }

  template<typename T = float, typename K = float>
  static constexpr auto smoothDamp(const T &current, const T &target, const K &rate) {
    return current + ((target - current) * rate);
  }

  template<typename T>
  static void hashCombine(size_t &seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x7a3699b9 + (seed << 4) + (seed >> 6);
  }

}

#endif //XE_MATHFUNC_HPP
