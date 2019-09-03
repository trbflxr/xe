//
// Created by FLXR on 6/14/2019.
//

#ifndef XE_VECMATH_HPP
#define XE_VECMATH_HPP


#include <cfloat>
#include <xe/math/mathfunc.hpp>
#include <xe/math/platformvecmath.hpp>

namespace xe {

  typedef detail::PlatformVector Vector;

  struct VectorConstants {
    static const Vector &Zero() {
      static Vector v = Vector::make(0.0f, 0.0f, 0.0f, 0.0f);
      return v;
    }

    static const Vector &One() {
      static Vector v = Vector::make(1.0f, 1.0f, 1.0f, 1.0f);
      return v;
    }

    static const Vector &Two() {
      static Vector v = Vector::make(2.0f, 2.0f, 2.0f, 2.0f);
      return v;
    }

    static const Vector &Half() {
      static Vector v = Vector::make(0.5f, 0.5f, 0.5f, 0.5f);
      return v;
    }

    static const Vector &Inf() {
      static Vector v = Vector::make(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
      return v;
    }

    static const Vector &MaskX() {
      static Vector v = Vector::make((uint) 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
      return v;
    }

    static const Vector &MaskY() {
      static Vector v = Vector::make((uint) 0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF);
      return v;
    }

    static const Vector &MaskZ() {
      static Vector v = Vector::make((uint) 0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF);
      return v;
    }

    static const Vector &MaskW() {
      static Vector v = Vector::make((uint) 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);
      return v;
    }

    static const Vector &SignMask() {
      static Vector v = Vector::make((uint) 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF);
      return v;
    }

  };

  template<>
  inline Vector lerp(const Vector &val1, const Vector &val2, const float &amt) {
    return (val2 - val1) * Vector::load1f(amt) + val1;
  }

}


#endif //XE_VECMATH_HPP
