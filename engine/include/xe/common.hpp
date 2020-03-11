//
// Created by FLXR on 9/2/2019.
//

#ifndef XE_COMMON_HPP
#define XE_COMMON_HPP

#include <cstdint>
#include <optional>

#define XE_SRC_ROOT_DIR XE_CURRENT_SRC_DIR

#if 0
  #ifdef XE_PLATFORM_WINDOWS
    #ifdef XE_STATIC
      #define XE_API __declspec(dllexport)
    #else
      #define XE_API __declspec(dllimport)
    #endif
  #endif
#endif


#define XE_API


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


namespace xe {

  struct Version {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
  };

  static constexpr Version cEngineVersion = {0, 1, 0};

  static constexpr uint32_t cMaxTextureUnits = 16;
  static constexpr uint32_t cMaxVertexAttribs = 16;
  static constexpr uint32_t cMaxFramebufferColorAttachments = 16;

  static constexpr uint32_t cMaxUniformBuffers = 16;
  static constexpr uint32_t cMaxShaderUniforms = 32;

  template<typename T>
  struct is_optional : std::false_type { };

  template<typename T>
  struct is_optional<std::optional<T>> : std::true_type {
  };

  template<typename T>
  inline constexpr bool is_optional_v = is_optional<T>::value;

  template<typename T>
  static T *to_address(T *obj) noexcept { return obj; }

  template<typename T>
  static T *to_address(T &obj) noexcept { return &obj; }

}

#endif //XE_COMMON_HPP
