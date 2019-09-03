//
// Created by FLXR on 6/13/2019.
//

#ifndef XE_PLATFORMVECMATH_HPP
#define XE_PLATFORMVECMATH_HPP


#include <xe/common.hpp>

#if defined(SIMD_CPU_ARCH_x86) || defined(SIMD_CPU_ARCH_x86_64)
#include <xe/math/ssevecmath.hpp>
namespace xe::detail {
  typedef SSEVector PlatformVector;
}
#else
  #error no generic math yet
#endif


#endif //XE_PLATFORMVECMATH_HPP
