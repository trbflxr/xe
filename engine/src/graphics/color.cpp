//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/graphics/color.hpp>

#include <iomanip>
#include <xe/assert.hpp>

namespace xe {

  const Color Color::Clear(0x00000000, Type::RGBA);
  const Color Color::Black(0x000000ff, Type::RGBA);
  const Color Color::Grey(0x808080);
  const Color Color::Silver(0xc0c0c0);
  const Color Color::White(0xffffff);
  const Color Color::Maroon(0x800000);
  const Color Color::Red(0xff0000);
  const Color Color::Olive(0x808000);
  const Color Color::Yellow(0xffff00);
  const Color Color::Green(0x00ff00);
  const Color Color::Lime(0x008000);
  const Color Color::Teal(0x008080);
  const Color Color::Aqua(0x00ffff);
  const Color Color::Navy(0x000080);
  const Color Color::Blue(0x0000ff);
  const Color Color::Purple(0x800080);
  const Color Color::Fuchsia(0xff00ff);

  Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }

  Color::Color(uint32_t hex, Type type) {
    switch (type) {
      case Type::RGBA: {
        r = static_cast<float>((uint8_t) (hex >> 24 & 0xff)) / 255.0f;
        g = static_cast<float>((uint8_t) (hex >> 16 & 0xff)) / 255.0f;
        b = static_cast<float>((uint8_t) (hex >> 8 & 0xff)) / 255.0f;
        a = static_cast<float>((uint8_t) (hex & 0xff)) / 255.0f;
        break;
      }
      case Type::ARGB: {
        r = static_cast<float>((uint8_t) (hex >> 16)) / 255.0f;
        g = static_cast<float>((uint8_t) (hex >> 8)) / 255.0f;
        b = static_cast<float>((uint8_t) (hex & 0xff)) / 255.0f;
        a = static_cast<float>((uint8_t) (hex >> 24)) / 255.0f;
        break;
      }
      case Type::RGB: {
        r = static_cast<float>((uint8_t) (hex >> 16)) / 255.0f;
        g = static_cast<float>((uint8_t) (hex >> 8)) / 255.0f;
        b = static_cast<float>((uint8_t) (hex & 0xff)) / 255.0f;
        a = 1.0f;
        break;
      }
      default: {
        XE_CORE_CRITICAL("[Color] Unknown Color type");
        throw std::runtime_error("Unknown Color type");
      }
    }
  }

  Color::Color(std::string hex, float a) : a(a) {
    if (hex[0] == '#') {
      hex.erase(0, 1);
    }

    XE_ASSERT(hex.size() == 6, "[Color] Bat hex format");

    auto hexValue = std::stoul(hex, nullptr, 16);
    r = static_cast<float>((hexValue >> 16) & 0xff) / 255.0f;
    g = static_cast<float>((hexValue >> 8) & 0xff) / 255.0f;
    b = static_cast<float>((hexValue >> 0) & 0xff) / 255.0f;
  }

  Color Color::add(const Color &other) const {
    return {r + other.r, g + other.g, b + other.b, a + other.a};
  }

  Color Color::subtract(const Color &other) const {
    return {r - other.r, g - other.g, b - other.b, a - other.a};
  }

  Color Color::multiply(const Color &other) const {
    return {r * other.r, g * other.g, b * other.b, a * other.a};
  }

  Color Color::divide(const Color &other) const {
    return {r / other.r, g / other.g, b / other.b, a / other.a};
  }

  Color Color::scale(float scalar) const {
    return {r * scalar, g * scalar, b * scalar, a * scalar};
  }

  Color Color::lerp(const Color &other, float progression) const {
    auto ta = *this * (1.0f - progression);
    auto tb = other * progression;
    return ta + tb;
  }

  Color Color::normalize() const {
    auto l = length();
    if (l == 0.0f) {
      XE_CORE_CRITICAL("[Color] Can't normalize a zero length vector");
      throw std::runtime_error("Can't normalize a zero length vector");
    }
    return {r / l, g / l, b / l, a / l};
  }

  float Color::lengthSquared() const {
    return r * r + g * g + b * b + a * a;
  }

  float Color::length() const {
    return std::sqrt(lengthSquared());
  }

  Color Color::smoothDamp(const Color &target, const Color &rate) const {
    return math::smoothDamp(*this, target, rate);
  }

  Color Color::getUnit() const {
    auto l = length();
    return {r / l, g / l, b / l, a / l};
  }

  uint32_t Color::getInt(Type type) const {
    switch (type) {
      case Type::RGBA:
        return (static_cast<uint8_t>(r * 255.0f) << 24)
               | (static_cast<uint8_t>(g * 255.0f) << 16)
               | (static_cast<uint8_t>(b * 255.0f) << 8)
               | (static_cast<uint8_t>(a * 255.0f)
                  & 0xff);
      case Type::ARGB:
        return (static_cast<uint8_t>(a * 255.0f) << 24)
               | (static_cast<uint8_t>(r * 255.0f) << 16)
               | (static_cast<uint8_t>(g * 255.0f) << 8)
               | (static_cast<uint8_t>(b * 255.0f)
                  & 0xff);
      case Type::RGB:
        return (static_cast<uint8_t>(r * 255.0f) << 16) | (static_cast<uint8_t>(g * 255.0f) << 8) | (static_cast<uint8_t>(b * 255.0f) & 0xff);
      default: {
        XE_CORE_CRITICAL("[Color] Unknown Color type");
        throw std::runtime_error("Unknown Color type");
      }
    }
  }

  std::string Color::getHex() const {
    std::stringstream stream;
    stream << "#";

    auto hexValue = ((static_cast<uint32_t>(r * 255.0f) & 0xff) << 16) +
                    ((static_cast<uint32_t>(g * 255.0f) & 0xff) << 8) +
                    ((static_cast<uint32_t>(b * 255.0f) & 0xff) << 0);
    stream << std::hex << std::setfill('0') << std::setw(6) << hexValue;

    return stream.str();
  }

  bool Color::operator==(const Color &other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
  }

  bool Color::operator!=(const Color &other) const {
    return !operator==(other);
  }

  Color operator+(const Color &left, const Color &right) {
    return left.add(right);
  }

  Color operator-(const Color &left, const Color &right) {
    return left.subtract(right);
  }

  Color operator*(const Color &left, const Color &right) {
    return left.multiply(right);
  }

  Color operator/(const Color &left, const Color &right) {
    return left.divide(right);
  }

  Color operator+(float value, const Color &left) {
    return Color(value, value, value, 0.0f).add(left);
  }

  Color operator-(float value, const Color &left) {
    return Color(value, value, value, 0.0f).subtract(left);
  }

  Color operator*(float value, const Color &left) {
    return Color(value, value, value).multiply(left);
  }

  Color operator/(float value, const Color &left) {
    return Color(value, value, value).divide(left);
  }

  Color operator+(const Color &left, float value) {
    return left.add({value, value, value, 0.0f});
  }

  Color operator-(const Color &left, float value) {
    return left.subtract({value, value, value, 0.0f});
  }

  Color operator*(const Color &left, float value) {
    return left.multiply({value, value, value});
  }

  Color operator/(const Color &left, float value) {
    return left.divide({value, value, value});
  }

  Color &Color::operator+=(const Color &other) {
    return *this = add(other);
  }

  Color &Color::operator-=(const Color &other) {
    return *this = subtract(other);
  }

  Color &Color::operator*=(const Color &other) {
    return *this = multiply(other);
  }

  Color &Color::operator/=(const Color &other) {
    return *this = divide(other);
  }

  Color &Color::operator+=(float value) {
    return *this = add({value, value, value, 0.0f});
  }

  Color &Color::operator-=(float value) {
    return *this = subtract({value, value, value, 0.0f});
  }

  Color &Color::operator*=(float value) {
    return *this = multiply({value, value, value});
  }

  Color &Color::operator/=(float value) {
    return *this = divide({value, value, value});
  }

}
