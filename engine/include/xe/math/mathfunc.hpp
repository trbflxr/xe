//
// Created by FLXR on 6/14/2019.
//

#ifndef XE_MATHFUNC_HPP
#define XE_MATHFUNC_HPP

#include <cmath>

namespace xe::math {

  static constexpr const float Pi = 3.1415926535897932f;
  static constexpr const float PiTwo = 6.28318530717959f;
  static constexpr const float PiHalf = 1.57079632679f;
  static constexpr const float RPi = 0.31830988618f;
  static constexpr const float RPiTwo = 0.159154943091895f;
  static constexpr const float RPiHalf = 0.636619772367581f;

  static constexpr const float E = 2.71828182845904523536f;
  static constexpr const float RLn2 = 1.44269504088896f;
  static constexpr const float RadToDeg = 57.2957795130823f;
  static constexpr const float DegToRad = 0.0174532925199433f;

  template<typename T>
  static constexpr T rad(T deg) {
    return deg * static_cast<T>(Pi) / static_cast<T>(180.0);
  }

  template<typename T>
  static constexpr T deg(T rad) {
    return rad * static_cast<T>(180.0) / static_cast<T>(Pi);
  }

  template<typename T>
  static constexpr T select(const T &cmp, const T &greaterOrEqualToZero, const T &lessZero) {
    return cmp >= (T) (0) ? greaterOrEqualToZero : lessZero;
  }

  static void sincos(float *outSin, float *outCos, float angle) {
    if ((angle < 0.0f) || (angle >= PiTwo)) {
      angle -= floorf(angle * RPiTwo) * PiTwo;
    }

    angle = Pi - angle;
    float sign = -1.0f;
    if (fabsf(angle) >= PiHalf) {
      angle = select(angle, Pi, -Pi) - angle;
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
