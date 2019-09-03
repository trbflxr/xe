//
// Created by FLXR on 9/2/2019.
//

#ifndef XE_COMMON_HPP
#define XE_COMMON_HPP

#ifdef _WIN32
  #define XE_PLATFORM_WINDOWS
#endif


#ifdef XE_PLATFORM_WINDOWS
  #ifdef XE_STATIC
    #define XE_API __declspec(dllexport)
  #else
    #define XE_API __declspec(dllimport)
  #endif
#endif


//SIMD levels
#define SIMD_CPU_ARCH_OTHER   0
#define SIMD_CPU_ARCH_x86     1
#define SIMD_CPU_ARCH_x86_64  2

#define SIMD_LEVEL_NONE       0
#define SIMD_LEVEL_x86_SSE    1
#define SIMD_LEVEL_x86_SSE2   2
#define SIMD_LEVEL_x86_SSE3   3
#define SIMD_LEVEL_x86_SSSE3  4
#define SIMD_LEVEL_x86_SSE4_1 5
#define SIMD_LEVEL_x86_SSE4_2 6
#define SIMD_LEVEL_x86_AVX    7
#define SIMD_LEVEL_x86_AVX2   8

//CPU architecture
#if (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64)) || defined(__x86_64__)
  #define SIMD_CPU_ARCH SIMD_CPU_ARCH_x86_64
#elif defined(_M_X86) || defined(__i386__) || defined(_X86_) || defined(_M_IX86)
  #define SIMD_CPU_ARCH SIMD_CPU_ARCH_x86
#else
  #define SIMD_CPU_ARCH SIMD_CPU_ARCH_OTHER
#endif

//supported SIMD features
#if defined(SIMD_CPU_ARCH_x86) || defined(SIMD_CPU_ARCH_x86_64)
  #if defined(INSTRSET)
    #define SIMD_SUPPORTED_LEVEL INSTRSET
  #elif defined(__AVX2__)
    #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_x86_AVX2
  #elif defined(__AVX__)
    #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_x86_AVX
  #elif defined(__SSE4_2__)
    #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_x86_SSE4_2
  #elif defined(__SSE4_1__)
    #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_x86_SSE4_1
  #elif defined(__SSSE3__)
    #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_x86_SSSE3
  #elif defined(__SSE3__)
    #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_x86_SSE3
  #elif defined(__SSE2__) || defined(SIMD_CPU_ARCH_x86_64)
    #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_x86_SSE2
  #elif defined(__SSE__)
    #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_x86_SSE
  #elif defined(_M_IX86_FP)
    #define SIMD_SUPPORTED_LEVEL _M_IX86_FP
  #else
    #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_NONE
  #endif
#else
  #define SIMD_SUPPORTED_LEVEL SIMD_LEVEL_NONE
#endif


#define BIT(x) (1 << (x))


#define XE_CORE       BIT(0)
#define XE_CLIENT     BIT(1)


#if defined(__clang__) || defined(__GNUC__)
  #define XE_FMT_ARGS(fmt)      __attribute__((format(printf, fmt, fmt + 1)))
  #define XE_FMT_LIST(fmt)      __attribute__((format(printf, fmt, 0)))
#else
  #define XE_FMT_ARGS(fmt)
  #define XE_FMT_LIST(fmt)
#endif


typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned uint32;
typedef unsigned long long uint64;

typedef uint8 byte;
typedef uint32 uint;

#ifdef _WIN32
  #ifdef _WIN64
typedef uint64 size_t;
  #else
typedef uint32 size_t;
  #endif
#endif


namespace xe {

  struct Params {
    struct Window {
      uint width;
      uint height;
      const char *title;
      bool swapInterval;

      uint iconWidth;
      uint iconHeight;
      byte *iconPixels = nullptr;
    };
  };

  void XE_API init();
  void XE_API shutdown();

}

#endif //XE_COMMON_HPP
