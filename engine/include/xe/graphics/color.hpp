//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_COLOR_HPP
#define XE_COLOR_HPP

#include <xe/math/mathfunc.hpp>
#include <xe/serialization/node.hpp>

namespace xe {

  struct XE_API Color {
  public:
    enum class Type {
      RGBA,
      ARGB,
      RGB
    };

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;

    Color() = default;
    Color(float r, float g, float b, float a = 1.0f);
    Color(uint32_t hex, Type type = Type::RGB);
    Color(std::string hex, float a = 1.0f);

    Color add(const Color &other) const;
    Color subtract(const Color &other) const;
    Color multiply(const Color &other) const;
    Color divide(const Color &other) const;

    Color scale(float scalar) const;
    Color lerp(const Color &other, float progression) const;
    Color normalize() const;

    float lengthSquared() const;
    float length() const;

    Color smoothDamp(const Color &target, const Color &rate) const;

    Color getUnit() const;

    uint32_t getInt(Type type = Type::RGBA) const;

    std::string getHex() const;

    bool operator==(const Color &other) const;
    bool operator!=(const Color &other) const;

    friend Color operator+(const Color &left, const Color &right);
    friend Color operator-(const Color &left, const Color &right);
    friend Color operator*(const Color &left, const Color &right);
    friend Color operator/(const Color &left, const Color &right);
    friend Color operator+(float value, const Color &left);
    friend Color operator-(float value, const Color &left);
    friend Color operator*(float value, const Color &left);
    friend Color operator/(float value, const Color &left);
    friend Color operator+(const Color &left, float value);
    friend Color operator-(const Color &left, float value);
    friend Color operator*(const Color &left, float value);
    friend Color operator/(const Color &left, float value);

    Color &operator+=(const Color &other);
    Color &operator-=(const Color &other);
    Color &operator*=(const Color &other);
    Color &operator/=(const Color &other);
    Color &operator+=(float value);
    Color &operator-=(float value);
    Color &operator*=(float value);
    Color &operator/=(float value);

    template<typename OStream>
    friend OStream &operator<<(OStream &os, const Color &c) { return os << "Color(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")"; }

    friend const Node &operator>>(const Node &node, Color &color) {
      std::string hex;
      node["hex"].get(hex);
      node["alpha"].get(color.a);
      color = {hex, color.a};
      return node;
    }

    friend Node &operator<<(Node &node, const Color &color) {
      node["hex"].set(color.getHex());
      node["alpha"].set(color.a);
      return node;
    }

    static const Color Clear;
    static const Color Black;
    static const Color Grey;
    static const Color Silver;
    static const Color White;
    static const Color Maroon;
    static const Color Red;
    static const Color Olive;
    static const Color Yellow;
    static const Color Green;
    static const Color Lime;
    static const Color Teal;
    static const Color Aqua;
    static const Color Navy;
    static const Color Blue;
    static const Color Purple;
    static const Color Fuchsia;
  };

}

namespace std {
  template<>
  struct hash<xe::Color> {
    size_t operator()(const xe::Color &color) const {
      size_t seed = 0;
      xe::math::hashCombine(seed, color.r);
      xe::math::hashCombine(seed, color.g);
      xe::math::hashCombine(seed, color.b);
      xe::math::hashCombine(seed, color.a);
      return seed;
    }
  };
}

#endif //XE_COLOR_HPP
