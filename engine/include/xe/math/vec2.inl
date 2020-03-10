//
// Created by trbflxr on 3/5/2020.
//

#ifndef XEVK_VEC2_INL
#define XEVK_VEC2_INL

namespace xe {

  template<typename T>
  constexpr tvec2<T>::tvec2(T x, T y) : x(x), y(y) { }

  template<typename T>
  constexpr tvec2<T>::tvec2(T value) : x(value), y(value) { }

  template<typename T>
  template<typename K>
  constexpr tvec2<T>::tvec2(const tvec2<K> &other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) { }

  template<typename T>
  constexpr auto tvec2<T>::normalize() const {
    const T lenSq = lengthSquared();
    return (*this) * rsqrt(lenSq);
  }

  template<typename T>
  constexpr bool tvec2<T>::isNormalized() const {
    return std::abs(1 - lengthSquared()) < 0;
  }

  template<typename T>
  constexpr auto tvec2<T>::max() const {
    return std::max(x, y);
  }

  template<typename T>
  constexpr auto tvec2<T>::min() const {
    return std::min(x, y);
  }

  template<typename T>
  constexpr auto tvec2<T>::absMax() const {
    return std::max(std::abs(x), std::abs(y));
  }

  template<typename T>
  constexpr auto tvec2<T>::absMin() const {
    return std::min(std::abs(x), std::abs(y));
  }

  template<typename T>
  constexpr auto tvec2<T>::abs() const {
    return tvec2<T>(std::abs(x), std::abs(y));
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::min(const tvec2<K> &other) const {
    return tvec2<decltype(std::min(x, other.x))>(std::min(x, other.x), std::min(y, other.y));
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::max(const tvec2<K> &other) const {
    return tvec2<decltype(std::max(x, other.x))>(std::max(x, other.x), std::max(y, other.y));
  }

  template<typename T>
  constexpr auto tvec2<T>::reciprocal() const {
    return tvec2<T>(math::reciprocal(x), math::reciprocal(y));
  }

  template<typename T>
  constexpr auto tvec2<T>::rotate(T angle) const {
    return tvec2<decltype(x * angle)>(x * std::cos(angle) - y * std::sin(angle),
                                      x * std::sin(angle) + y * std::cos(angle));
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::dot(const tvec2<K> &other) const {
    return x * other.x + y * other.y;
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::dotToVector(const tvec2<K> &other) const {
    return tvec2<decltype(x * other.x)>(dot(other));
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::cross(const tvec2<K> &other) const {
    return x * other.y - y * other.x;
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::dist(const tvec2<K> &other) const {
    return std::sqrt(distSquared(other));
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::distSquared(const tvec2<K> &other) const {
    return (other - *this).lengthSquared();
  }

  template<typename T>
  constexpr auto tvec2<T>::length() const {
    return std::sqrt(lengthSquared());
  }

  template<typename T>
  constexpr auto tvec2<T>::lengthSquared() const {
    return x * x + y * y;
  }

  template<typename T>
  constexpr auto tvec2<T>::toDeg() const {
    return tvec2<T>(static_cast<T>(XE_MATH_RAD_TO_DEG * x), static_cast<T>(XE_MATH_RAD_TO_DEG * y));
  }

  template<typename T>
  constexpr auto tvec2<T>::toRad() const {
    return tvec2<T>(static_cast<T>(XE_MATH_DEG_TO_RAD * x), static_cast<T>(XE_MATH_DEG_TO_RAD * y));
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator+(const tvec2<K> &other) const {
    return tvec2<decltype(x + other.x)>(x + other.x, y + other.y);
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator-(const tvec2<K> &other) const {
    return tvec2<decltype(x - other.x)>(x - other.x, y - other.y);
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator*(const tvec2<K> &other) const {
    return tvec2<decltype(x * other.x)>(x * other.x, y * other.y);
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator/(const tvec2<K> &other) const {
    return tvec2<decltype(x / other.x)>(x / other.x, y / other.y);
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator+(K amt) const {
    return tvec2<decltype(x + amt)>(x + amt, y + amt);
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator-(K amt) const {
    return tvec2<decltype(x - amt)>(x - amt, y - amt);
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator*(K amt) const {
    return tvec2<decltype(x * amt)>(x * amt, y * amt);
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator/(K amt) const {
    return tvec2<decltype(x / amt)>(x / amt, y / amt);
  }

  template<typename T>
  constexpr auto tvec2<T>::operator-() const {
    return tvec2<T>(-x, -y);
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator+=(const tvec2<K> &other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator-=(const tvec2<K> &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator*=(const tvec2<K> &other) {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator/=(const tvec2<K> &other) {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator+=(K val) {
    x += val;
    y += val;
    return *this;
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator-=(K val) {
    x -= val;
    y -= val;
    return *this;
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator*=(K val) {
    x *= val;
    y *= val;
    return *this;
  }

  template<typename T>
  template<typename K>
  constexpr auto tvec2<T>::operator/=(K val) {
    x /= val;
    y /= val;
    return *this;
  }

  template<typename T>
  template<typename K>
  constexpr bool tvec2<T>::operator==(const tvec2<K> &other) const {
    return x == other.x && y == other.y;
  }

  template<typename T>
  template<typename K>
  constexpr bool tvec2<T>::operator!=(const tvec2<K> &other) const {
    return x != other.x || y != other.y;
  }

}

#endif //XEVK_VEC2_INL
