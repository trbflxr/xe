//
// Created by FLXR on 6/13/2019.
//

#ifndef XE_SSEVECMATH_HPP
#define XE_SSEVECMATH_HPP


#include <cstring>
#include <xe/common.hpp>
#include <xe/math/mathfunc.hpp>

#if defined(SIMD_CPU_ARCH_x86) || defined(SIMD_CPU_ARCH_x86_64)
  #if SIMD_SUPPORTED_LEVEL >= SIMD_LEVEL_x86_AVX2
    #ifdef __GNUC__
      #include <x86intrin.h>
    #else
      #include <immintrin.h>
    #endif
  #elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_AVX
    #include <immintrin.h>
  #elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE4_2
    #include <nmmintrin.h>
  #elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE4_1
    #include <smmintrin.h>
  #elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSSE3
    #include <tmmintrin.h>
  #elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE3
    #include <pmmintrin.h>
  #elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE2
    #include <emmintrin.h>
  #elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE
    #include <xmmintrin.h>
  #endif
#endif

namespace xe::detail {

#define SSEVector_SHUFFLEMASK(a0, a1, b2, b3)       ((a0) | ((a1) << 2) | ((b2) << 4) | ((b3) << 6))
#define SSEVector_Shuffle(vec1, vec2, x, y, z, w)   _mm_shuffle_ps(vec1, vec2, SSEVector_SHUFFLEMASK(x, y, z, w))
#define SSEVector_Swizzle(vec, x, y, z, w)          _mm_shuffle_ps(vec, vec,   SSEVector_SHUFFLEMASK(x, y, z, w))
#define SSEVector_Shuffle_0101(vec1, vec2)          _mm_movelh_ps(vec1, vec2)
#define SSEVector_Shuffle_2323(vec1, vec2)          _mm_movehl_ps(vec2, vec1)

  struct SSEVector {
  public:
    static inline void matrixMul(void *result, const void *mat1, const void *mat2) {
      const SSEVector *m1 = static_cast<const SSEVector *>(mat1);
      const SSEVector *m2 = static_cast<const SSEVector *>(mat2);
      SSEVector *r = static_cast<SSEVector *>(result);
      SSEVector temp{ };
      SSEVector r0{ };
      SSEVector r1{ };
      SSEVector r2{ };
      SSEVector r3{ };

      temp = m1[0].replicate(0) * m2[0];
      temp = m1[0].replicate(1).mad(m2[1], temp);
      temp = m1[0].replicate(2).mad(m2[2], temp);
      r0 = m1[0].replicate(3).mad(m2[3], temp);

      temp = m1[1].replicate(0) * m2[0];
      temp = m1[1].replicate(1).mad(m2[1], temp);
      temp = m1[1].replicate(2).mad(m2[2], temp);
      r1 = m1[1].replicate(3).mad(m2[3], temp);

      temp = m1[2].replicate(0) * m2[0];
      temp = m1[2].replicate(1).mad(m2[1], temp);
      temp = m1[2].replicate(2).mad(m2[2], temp);
      r2 = m1[2].replicate(3).mad(m2[3], temp);

      temp = m1[3].replicate(0) * m2[0];
      temp = m1[3].replicate(1).mad(m2[1], temp);
      temp = m1[3].replicate(2).mad(m2[2], temp);
      r3 = m1[3].replicate(3).mad(m2[3], temp);

      r[0] = r0;
      r[1] = r1;
      r[2] = r2;
      r[3] = r3;
    }

    static inline float matrixDeterminant3x3Vector(const SSEVector *m) {
      float M[4][4];
      for (uint32_t i = 0; i < 4; i++) {
        m[i].store4f(M[i]);
      }
      return M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
             M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
             M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1]);
    }

    static inline float matrixDeterminant4x4(float *outS, float *outC, const void *matrix) {
      float sValues[6];
      float cValues[6];

      float *s = outS == nullptr ? sValues : outS;
      float *c = outC == nullptr ? cValues : outC;

      const SSEVector *m = static_cast<const SSEVector *>(matrix);
      float mat[4][4];
      for (uint32_t i = 0; i < 4; i++) {
        m[i].store4f(mat[i]);
      }

      s[0] = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
      s[1] = mat[0][0] * mat[1][2] - mat[1][0] * mat[0][2];
      s[2] = mat[0][0] * mat[1][3] - mat[1][0] * mat[0][3];
      s[3] = mat[0][1] * m[1][2] - mat[1][1] * mat[0][2];
      s[4] = mat[0][1] * mat[1][3] - mat[1][1] * mat[0][3];
      s[5] = mat[0][2] * mat[1][3] - mat[1][2] * mat[0][3];

      c[0] = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];
      c[1] = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
      c[2] = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
      c[3] = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
      c[4] = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
      c[5] = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];

      return (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);
    }

    // Based on https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html
    static inline void matrixInverse(void *dest, const void *src) {
      const __m128 *mVec = static_cast<const __m128 *>(src);
      const float *m = static_cast<const float *>(src);

      const __m128 A = SSEVector_Shuffle_0101(mVec[0], mVec[1]);
      const __m128 B = SSEVector_Shuffle_2323(mVec[0], mVec[1]);
      const __m128 C = SSEVector_Shuffle_0101(mVec[2], mVec[3]);
      const __m128 D = SSEVector_Shuffle_2323(mVec[2], mVec[3]);

      const __m128 detA = _mm_set1_ps(m[0 * 4 + 0] * m[1 * 4 + 1] - m[0 * 4 + 1] * m[1 * 4 + 0]);
      const __m128 detB = _mm_set1_ps(m[0 * 4 + 2] * m[1 * 4 + 3] - m[0 * 4 + 3] * m[1 * 4 + 2]);
      const __m128 detC = _mm_set1_ps(m[2 * 4 + 0] * m[3 * 4 + 1] - m[2 * 4 + 1] * m[3 * 4 + 0]);
      const __m128 detD = _mm_set1_ps(m[2 * 4 + 2] * m[3 * 4 + 3] - m[2 * 4 + 3] * m[3 * 4 + 2]);

      __m128 D_C = mat2AdjMul(D, C);
      __m128 A_B = mat2AdjMul(A, B);
      __m128 X_ = _mm_sub_ps(_mm_mul_ps(detD, A), mat2Mul(B, D_C));
      __m128 W_ = _mm_sub_ps(_mm_mul_ps(detA, D), mat2Mul(C, A_B));

      __m128 detM = _mm_mul_ps(detA, detD);
      __m128 Y_ = _mm_sub_ps(_mm_mul_ps(detB, C), mat2MulAdj(D, A_B));
      __m128 Z_ = _mm_sub_ps(_mm_mul_ps(detC, B), mat2MulAdj(A, D_C));
      detM = _mm_add_ps(detM, _mm_mul_ps(detB, detC));

      __m128 tr = horizontalAdd(_mm_mul_ps(A_B, SSEVector_Swizzle(D_C, 0, 2, 1, 3)));
      detM = _mm_sub_ps(detM, tr);

      const __m128 adjSignMask = _mm_setr_ps(1.0f, -1.0f, -1.0f, 1.0f);
      __m128 rDetM = _mm_div_ps(adjSignMask, detM);

      X_ = _mm_mul_ps(X_, rDetM);
      Y_ = _mm_mul_ps(Y_, rDetM);
      Z_ = _mm_mul_ps(Z_, rDetM);
      W_ = _mm_mul_ps(W_, rDetM);

      __m128 *rmVec = (__m128 *) dest;
      rmVec[0] = SSEVector_Shuffle(X_, Y_, 3, 1, 3, 1);
      rmVec[1] = SSEVector_Shuffle(X_, Y_, 2, 0, 2, 0);
      rmVec[2] = SSEVector_Shuffle(Z_, W_, 3, 1, 3, 1);
      rmVec[3] = SSEVector_Shuffle(Z_, W_, 2, 0, 2, 0);
    }

    static inline void
    createTransformMatrix(void *dest, const SSEVector &translation, const SSEVector &rot, const SSEVector &scale) {

      static const SSEVector MASK_W(SSEVector::make((uint32_t) 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0));

      const SSEVector rot2 = rot + rot;
      const SSEVector rs12 = rot * rot2;
      SSEVector rs22{ };
      rs22.data_ = _mm_mul_ps(SSEVector_Swizzle(rot.data_, 0, 1, 0, 3), SSEVector_Swizzle(rot2.data_, 1, 2, 2, 3));
      const SSEVector rs32 = rot.replicate(3) * rot2;

      const float xx2 = rs12[0];
      const float yy2 = rs12[1];
      const float zz2 = rs12[2];
      const float xy2 = rs22[0];
      const float yz2 = rs22[1];
      const float xz2 = rs22[2];
      const float xw2 = rs32[0];
      const float yw2 = rs32[1];
      const float zw2 = rs32[2];

      const SSEVector newScale = scale.select(MASK_W, SSEVector::load1f(1.0f));
      SSEVector *mat = static_cast<SSEVector *>(dest);
      mat[0] = newScale * make((1.0f - (yy2 + zz2)), (xy2 - zw2), (xz2 + yw2), translation[0]);
      mat[1] = newScale * make((xy2 + zw2), (1.0f - (xx2 + zz2)), (yz2 - xw2), translation[1]);
      mat[2] = newScale * make((xz2 - yw2), (yz2 + xw2), (1.0f - (xx2 + yy2)), translation[2]);
      mat[3] = make(0.0f, 0.0f, 0.0f, 1.0f);
    }

    static inline SSEVector make(uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
      union {
        __m128 vecf;
        __m128i veci;
      } vecData{ };
      vecData.veci = _mm_setr_epi32(x, y, z, w);

      SSEVector result{ };
      result.data_ = vecData.vecf;
      return result;
    }

    static inline const SSEVector mask(uint32_t index) {
      static const SSEVector masks[4] = {SSEVector::make((uint32_t) 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
                                         SSEVector::make((uint32_t) 0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF),
                                         SSEVector::make((uint32_t) 0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF),
                                         SSEVector::make((uint32_t) 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0)};
      return masks[index];
    }

    static inline SSEVector make(float x, float y, float z, float w) {
      SSEVector vec{ };
      vec.data_ = _mm_setr_ps(x, y, z, w);
      return vec;
    }

    static inline SSEVector load4f(const float *values) {
      SSEVector vec{ };
      vec.data_ = _mm_loadu_ps(values);
      return vec;
    }

    static inline SSEVector load3f(const float *values, float w) {
      return make(values[0], values[1], values[2], w);
    }

    static inline SSEVector load1f(float val) {
      SSEVector vec{ };
      vec.data_ = _mm_set1_ps(val);
      return vec;
    }

    static inline SSEVector loadAligned(const float *values) {
      SSEVector vec{ };
      vec.data_ = _mm_load_ps(values);
      return vec;
    }

    static inline SSEVector set(float x, float y, float z) {
      return make(x, y, z, 0.0f);
    }

    static inline SSEVector set(float x, float y, float z, float w) {
      return make(x, y, z, w);
    }

    inline void store4f(float *result) const {
      _mm_storeu_ps(result, data_);
    }

    inline void store3f(float *result) const {
      memcpy(result, &data_, sizeof(float) * 3);
    }

    inline void store1f(float *result) const {
      _mm_store_ss(result, data_);
    }

    inline void storeAligned(float *result) const {
      _mm_store_ps(result, data_);
    }

    inline void storeAlignedStreamed(float *result) const {
      _mm_stream_ps(result, data_);
    }

    inline SSEVector replicate(uint32_t index) const {
      return SSEVector::load1f((*this)[index]);
    }

    inline SSEVector abs() const {
      static const SSEVector SIGN_MASK(SSEVector::make((uint32_t) 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF));

      SSEVector vec{ };
      vec.data_ = _mm_and_ps(data_, SIGN_MASK.data_);

      return vec;
    }

    inline SSEVector sign() const {
      static const SSEVector SIGN_MASK(SSEVector::make((uint32_t) 0x80000000, 0x80000000, 0x80000000, 0x80000000));

      SSEVector vec{ };
      vec.data_ = _mm_and_ps(data_, SIGN_MASK.data_);

      return vec;
    }

    inline SSEVector min(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_min_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector max(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_max_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector neg() const {
      SSEVector vec{ };
      vec.data_ = _mm_xor_ps(data_, _mm_set1_ps(-0.0f));

      return vec;
    }

    inline SSEVector operator-() const {
      return neg();
    }

    inline SSEVector dot3(const SSEVector &other) const {
      const __m128 mul = _mm_mul_ps(data_, other.data_);
      const __m128 x = _mm_shuffle_ps(mul, mul, SSEVector_SHUFFLEMASK(0, 0, 0, 0));
      const __m128 y = _mm_shuffle_ps(mul, mul, SSEVector_SHUFFLEMASK(1, 1, 1, 1));
      const __m128 z = _mm_shuffle_ps(mul, mul, SSEVector_SHUFFLEMASK(2, 2, 2, 2));

      SSEVector vec{ };
      vec.data_ = _mm_add_ps(x, _mm_add_ps(y, z));

      return vec;
    }

    inline SSEVector dot4(const SSEVector &other) const {
      const __m128 t0 = _mm_mul_ps(data_, other.data_);

      SSEVector vec{ };
      vec.data_ = horizontalAdd(t0);

      return vec;
    }

    inline SSEVector cross3(const SSEVector &other) const {
      const __m128 t0 = _mm_shuffle_ps(data_, data_, SSEVector_SHUFFLEMASK(1, 2, 0, 3));
      const __m128 t1 = _mm_shuffle_ps(other.data_, other.data_, SSEVector_SHUFFLEMASK(1, 2, 0, 3));

      SSEVector vec{ };
      vec.data_ = _mm_sub_ps(_mm_mul_ps(data_, t1), _mm_mul_ps(other.data_, t0));
      vec.data_ = _mm_shuffle_ps(vec.data_, vec.data_, SSEVector_SHUFFLEMASK(1, 2, 0, 3));

      return vec;
    }

    inline SSEVector pow(const SSEVector &exp) const {
      return make(powf((*this)[0], exp[0]),
                  powf((*this)[1], exp[1]),
                  powf((*this)[2], exp[2]),
                  powf((*this)[3], exp[3]));
    }

    inline SSEVector rsqrt() const {
      static const SSEVector ONE(SSEVector::load1f(1.0f));

      SSEVector vec{ };
      vec.data_ = _mm_div_ps(ONE.data_, _mm_sqrt_ps(data_));

      return vec;
    }

    inline SSEVector reciprocal() const {
      static const SSEVector ONE(SSEVector::load1f(1.0f));

      SSEVector vec{ };
      vec.data_ = _mm_div_ps(ONE.data_, data_);

      return vec;
    }

    inline SSEVector rlen4() const {
      return dot4(*this).rsqrt();
    }

    inline SSEVector rlen3() const {
      return dot3(*this).rsqrt();
    }

    inline SSEVector normalize4() const {
      return (*this) * rlen4();
    }

    inline SSEVector normalize3() const {
      return (*this) * rlen3();
    }

    inline void sincos(SSEVector *outSin, SSEVector *outCos) const {
      float outSinValues[4];
      float outCosValues[4];

      math::sincos(&outSinValues[0], &outCosValues[0], (*this)[0]);
      math::sincos(&outSinValues[1], &outCosValues[1], (*this)[1]);
      math::sincos(&outSinValues[2], &outCosValues[2], (*this)[2]);
      math::sincos(&outSinValues[3], &outCosValues[3], (*this)[3]);

      *outSin = SSEVector::load4f(outSinValues);
      *outCos = SSEVector::load4f(outCosValues);
    }

    inline SSEVector quatMul(const SSEVector &other) const {
      static const SSEVector MASK(SSEVector::make(0.0f, 0.0f, 0.0f, -0.0f));

      SSEVector comp1{ };
      comp1.data_ = _mm_mul_ps(SSEVector_Swizzle(data_, 0, 1, 2, 0), SSEVector_Swizzle(other.data_, 3, 3, 3, 0));

      SSEVector comp2{ };
      comp2.data_ = _mm_mul_ps(SSEVector_Swizzle(data_, 1, 2, 0, 1), SSEVector_Swizzle(other.data_, 2, 0, 1, 1));

      SSEVector comp3{ };
      comp3.data_ = _mm_mul_ps(SSEVector_Swizzle(data_, 2, 0, 1, 2), SSEVector_Swizzle(other.data_, 1, 2, 0, 2));

      return replicate(3) * other - comp3 + ((comp1 + comp2) ^ MASK);
    }

    inline SSEVector quatRotateVec(const SSEVector &vec) const {
      SSEVector tmp = SSEVector::load1f(2.0f) * cross3(vec);

      return vec + (tmp * replicate(3)) + cross3(tmp);
    }

    inline SSEVector mad(const SSEVector &mul, const SSEVector &add) const {
      SSEVector vec{ };
      vec.data_ = _mm_add_ps(_mm_mul_ps(data_, mul.data_), add.data_);

      return vec;
    }

    inline SSEVector transform(const void *matrix) const {
      const SSEVector *m = static_cast<const SSEVector *>(matrix);

      return make(dot4(m[0])[0], dot4(m[1])[0], dot4(m[2])[0], dot4(m[3])[0]);
    }

    inline SSEVector operator+(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_add_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator-(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_sub_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator*(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_mul_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator/(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_div_ps(data_, other.data_);

      return vec;
    }

    inline bool isZero3f() const {
      return !(_mm_movemask_ps(data_) & 0x07);
    }

    inline bool isZero4f() const {
      return !_mm_movemask_ps(data_);
    }

    inline SSEVector operator==(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_cmpeq_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector equals(const SSEVector &other, float errorMargin) const {
      return (*this - other).abs() < SSEVector::load1f(errorMargin);
    }

    inline SSEVector notEquals(const SSEVector &other, float errorMargin) const {
      return (*this - other).abs() >= SSEVector::load1f(errorMargin);
    }

    inline SSEVector operator!=(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_cmpneq_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator>(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_cmpgt_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator>=(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_cmpge_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator<(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_cmplt_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator<=(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_cmple_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator|(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_or_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator&(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_and_ps(data_, other.data_);

      return vec;
    }

    inline SSEVector operator^(const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_xor_ps(data_, other.data_);

      return vec;
    }

    inline float operator[](uint32_t index) const {
      return ((float *) &data_)[index];
    }

    inline SSEVector select(const SSEVector &mask, const SSEVector &other) const {
      SSEVector vec{ };
      vec.data_ = _mm_xor_ps(other.data_, _mm_and_ps(mask.data_, _mm_xor_ps(data_, other.data_)));

      return vec;
    }

  private:
    static inline __m128 horizontalAdd(__m128 t0) {
#if SIMD_SUPPORTED_LEVEL >= SIMD_LEVEL_x86_SSSE3
      const __m128 t1 = _mm_hadd_ps(t0, t0);
      return _mm_hadd_ps(t1, t1);
#else
      const __m128 t1 = _mm_shuffle_ps(t0, t0, SSEVector_SHUFFLEMASK(2, 3, 0, 1));
      const __m128 t2 = _mm_add_ps(t1, t0);
      const __m128 t3 = _mm_shuffle_ps(t2, t2, SSEVector_SHUFFLEMASK(1, 2, 3, 0));
      return _mm_add_ps(t3, t2);
#endif
    }

    static inline __m128 mat2Mul(__m128 vec1, __m128 vec2) {
      return _mm_add_ps(_mm_mul_ps(vec1, SSEVector_Swizzle(vec2, 0, 3, 0, 3)),
                        _mm_mul_ps(SSEVector_Swizzle(vec1, 1, 0, 3, 2), SSEVector_Swizzle(vec2, 2, 1, 2, 1)));
    }
    // 2x2 row major Matrix adjugate multiply (A#)*B
    static inline __m128 mat2AdjMul(__m128 vec1, __m128 vec2) {
      return _mm_sub_ps(_mm_mul_ps(SSEVector_Swizzle(vec1, 3, 3, 0, 0), vec2),
                        _mm_mul_ps(SSEVector_Swizzle(vec1, 1, 1, 2, 2), SSEVector_Swizzle(vec2, 2, 3, 0, 1)));

    }
    // 2x2 row major Matrix multiply adjugate A*(B#)
    static inline __m128 mat2MulAdj(__m128 vec1, __m128 vec2) {
      return _mm_sub_ps(_mm_mul_ps(vec1, SSEVector_Swizzle(vec2, 3, 0, 3, 0)),
                        _mm_mul_ps(SSEVector_Swizzle(vec1, 1, 0, 3, 2), SSEVector_Swizzle(vec2, 2, 1, 2, 1)));
    }

  private:
    __m128 data_;
  };

}


#endif //XE_SSEVECMATH_HPP
