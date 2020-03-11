//
// Created by trbflxr on 2/29/2020.
//

#ifndef XE_VEC2_HPP
#define XE_VEC2_HPP

#include <xe/common.hpp>
#include <xe/math/vecmath.hpp>
#include <xe/serialization/node.hpp>

namespace xe {

  class vec3;
  class vec4;

  template<typename T>
  class XE_API tvec2 {
  public:
    T x = 0;
    T y = 0;

    constexpr tvec2() = default;
    constexpr tvec2(T x, T y);
    constexpr explicit tvec2(T value);
    explicit tvec2(const vec3 &vec);
    explicit tvec2(const vec4 &vec);

    template<typename K>
    constexpr tvec2(const tvec2<K> &other);

    constexpr auto normalize() const;
    constexpr bool isNormalized() const;

    constexpr auto max() const;
    constexpr auto min() const;
    constexpr auto absMax() const;
    constexpr auto absMin() const;

    constexpr auto abs() const;

    template<typename K>
    constexpr auto min(const tvec2<K> &other) const;

    template<typename K>
    constexpr auto max(const tvec2<K> &other) const;

    constexpr auto reciprocal() const;

    constexpr auto rotate(T angle) const;

    template<typename K>
    constexpr auto dot(const tvec2<K> &other) const;

    template<typename K>
    constexpr auto dotToVector(const tvec2<K> &other) const;

    template<typename K>
    constexpr auto cross(const tvec2<K> &other) const;

    template<typename K>
    constexpr auto dist(const tvec2<K> &other) const;

    template<typename K>
    constexpr auto distSquared(const tvec2<K> &other) const;

    constexpr auto length() const;
    constexpr auto lengthSquared() const;

    constexpr auto toDeg() const;
    constexpr auto toRad() const;

    Vector toVector() const { return toVector(0, 0); }

    template<typename K>
    Vector toVector(K z, K w) const {
      return Vector::make(static_cast<float>(x),
                          static_cast<float>(y),
                          static_cast<float>(z),
                          static_cast<float>(w));
    }

    template<typename K>
    Vector toVector(const tvec2<K> &other) const {
      return Vector::make(static_cast<float>(x),
                          static_cast<float>(y),
                          static_cast<float>(other.x),
                          static_cast<float>(other.y));
    }

    template<typename OStream>
    friend OStream &operator<<(OStream &os, const tvec2<T> &v) { return os << "vec2(" << v.x << ", " << v.y << ")"; }

    friend const Node &operator>>(const Node &node, tvec2<T> &vector) {
      node["x"].get(vector.x);
      node["y"].get(vector.y);
      return node;
    }

    friend Node &operator<<(Node &node, const tvec2<T> &vector) {
      node["x"].set(vector.x);
      node["y"].set(vector.y);
      return node;
    }

    template<typename K>
    constexpr auto operator+(const tvec2<K> &other) const;

    template<typename K>
    constexpr auto operator-(const tvec2<K> &other) const;

    template<typename K>
    constexpr auto operator*(const tvec2<K> &other) const;

    template<typename K>
    constexpr auto operator/(const tvec2<K> &other) const;


    template<typename K>
    constexpr auto operator+(K amt) const;

    template<typename K>
    constexpr auto operator-(K amt) const;

    template<typename K>
    constexpr auto operator*(K amt) const;

    template<typename K>
    constexpr auto operator/(K amt) const;


    constexpr auto operator-() const;

    template<typename K>
    constexpr auto operator+=(const tvec2<K> &other);

    template<typename K>
    constexpr auto operator-=(const tvec2<K> &other);

    template<typename K>
    constexpr auto operator*=(const tvec2<K> &other);

    template<typename K>
    constexpr auto operator/=(const tvec2<K> &other);


    template<typename K>
    constexpr auto operator+=(K val);

    template<typename K>
    constexpr auto operator-=(K val);

    template<typename K>
    constexpr auto operator*=(K val);

    template<typename K>
    constexpr auto operator/=(K val);


    template<typename K>
    constexpr bool operator==(const tvec2<K> &other) const;

    template<typename K>
    constexpr bool operator!=(const tvec2<K> &other) const;
  };

  using vec2  = tvec2<float>;
  using vec2i = tvec2<int32_t>;
  using vec2u = tvec2<uint32_t>;

}

#include <xe/math/vec2.inl>

#endif //XE_VEC2_HPP
