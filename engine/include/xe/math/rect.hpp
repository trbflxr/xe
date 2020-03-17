//
// Created by trbflxr on 3/17/2020.
//

#ifndef XE_RECT_HPP
#define XE_RECT_HPP

#include <xe/math/vec2.hpp>

namespace xe {

  template<typename T>
  class trect {
  public:
    T x;
    T y;
    T width;
    T height;

    constexpr trect();
    constexpr trect(T x, T y, T width, T height);

    template<typename K>
    constexpr trect(const tvec2<K> &position, const tvec2<K> &size);

    template<typename K>
    constexpr explicit trect(const trect<K> &other);

    template<typename K>
    constexpr bool intersects(const trect<K> &other) const;

    template<typename K>
    constexpr bool contains(const tvec2<K> &point) const;

    constexpr bool contains(const vec3 &point) const;

    template<typename K>
    constexpr bool contains(const trect<K> &other) const;

    constexpr auto getMinBound() const { return tvec2<decltype(x - width)>(x - width, y - height); }
    constexpr auto getMaxBound() const { return tvec2<decltype(x + width)>(x + width, y + height); }


    template<typename OStream>
    friend OStream &operator<<(OStream &os, const trect<T> &r) {
      return os << "rect(" << r.x << ", " << r.y << ", " << r.width << ", " << r.height << ")";
    }

    friend const Node &operator>>(const Node &node, trect<T> &r) {
      node["x"].get(r.x);
      node["y"].get(r.y);
      node["width"].get(r.width);
      node["height"].get(r.height);
      return node;
    }

    friend Node &operator<<(Node &node, const trect<T> &r) {
      node["x"].set(r.x);
      node["y"].set(r.y);
      node["width"].set(r.width);
      node["height"].set(r.height);
      return node;
    }

    template<typename K>
    constexpr bool operator==(const trect<K> &r) const;

    template<typename K>
    constexpr bool operator!=(const trect<K> &r) const;

    template<typename K>
    constexpr bool operator>(const trect<K> &r) const;

    template<typename K>
    constexpr bool operator<(const trect<K> &r) const;
  };

  using rect  = trect<float>;
  using recti = trect<int32_t>;
  using rectu = trect<uint32_t>;


  template<typename T>
  constexpr trect<T>::trect() :
      x(0),
      y(0),
      width(0),
      height(0) { }

  template<typename T>
  constexpr trect<T>::trect(T x, T y, T width, T height) :
      x(x),
      y(y),
      width(width),
      height(height) { }

  template<typename T>
  template<typename K>
  constexpr trect<T>::trect(const tvec2<K> &position, const tvec2<K> &size) :
      x(position.x),
      y(position.y),
      width(size.x),
      height(size.y) { }

  template<typename T>
  template<typename K>
  constexpr trect<T>::trect(const trect<K> &other):
      x(other.x),
      y(other.y),
      width(other.width),
      height(other.height) { }

  template<typename T>
  template<typename K>
  constexpr bool trect<T>::intersects(const trect<K> &other) const {
    return x < other.x + other.width && y < other.y + other.height && x + width > other.width && y + height > other.height;
  }

  template<typename T>
  template<typename K>
  constexpr bool trect<T>::contains(const tvec2<K> &point) const {
    return point > getMinBound() && point < getMaxBound();
  }

  template<typename T>
  constexpr bool trect<T>::contains(const vec3 &point) const {
    return contains(vec2(point));
  }

  template<typename T>
  template<typename K>
  constexpr bool trect<T>::contains(const trect<K> &other) const {
    return x >= other.x && y >= other.y &&
           x + width <= other.x + other.width &&
           y + height <= other.y + other.height;
  }

  template<typename T>
  template<typename K>
  constexpr bool trect<T>::operator==(const trect<K> &r) const {
    return x == r.x && y == r.y && width = r.width && height == r.height;
  }

  template<typename T>
  template<typename K>
  constexpr bool trect<T>::operator!=(const trect<K> &r) const {
    return (*this == r);
  }

  template<typename T>
  template<typename K>
  constexpr bool trect<T>::operator>(const trect<K> &r) const {
    return width > r.width && height > r.height;
  }

  template<typename T>
  template<typename K>
  constexpr bool trect<T>::operator<(const trect<K> &r) const {
    return width < r.width && height < r.height;
  }

}

#endif //XE_RECT_HPP
