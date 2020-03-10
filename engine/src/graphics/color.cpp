//
// Created by FLXR on 9/3/2019.
//

#include <xe/graphics/color.hpp>

namespace xe {

  Color::Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) { }
  Color::Color(const Color &o) : r(o.r), g(o.g), b(o.b), a(o.a) { }
  Color::Color(const vec4 &rgba) : r(rgba[0]), g(rgba[1]), b(rgba[2]), a(rgba[3]) { }
  Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
  Color::Color(uint32_t hex) {
    b = ((hex >> 0) & 255) / 255.0f;
    g = ((hex >> 8) & 255) / 255.0f;
    r = ((hex >> 16) & 255) / 255.0f;
    a = ((hex >> 24) & 255) / 255.0f;
  }

  uint32_t Color::hex() const {
    const uint32_t r1 = static_cast<uint32_t>(r * 255);
    const uint32_t g1 = static_cast<uint32_t>(g * 255);
    const uint32_t b1 = static_cast<uint32_t>(b * 255);
    const uint32_t a1 = static_cast<uint32_t>(a * 255);

    const uint32_t result = a1 << 24 | r1 << 16 | g1 << 8 | b1;
    return result;
  }

  float Color::luminance() const {
    return r * 0.3f + g * 0.59f + b * 0.11f;
  }

  Color Color::desaturate(float factor) const {
    const float lum = luminance();
    return lerp(*this, Color(lum, lum, lum, 0), factor);
  }

  Color Color::lerp(const Color &a, const Color &b, float alpha) {
    Color c = b - a;
    c *= alpha;
    c += a;
    return c;
  }

#define COLOR(name, r, g, b) \
  static Color name (r / 255.0f, g / 255.0f, b / 255.0f, 1.0f); \
  return name;

  Color Color::transparent() {
    static Color transparent = Color(0.0f, 0.0f, 0.0f, 0.0f);
    return transparent;
  }

  Color Color::black() { COLOR(black, 0, 0, 0); }
  Color Color::white() { COLOR(white, 255, 255, 255); }
  Color Color::red() { COLOR(red, 255, 0, 0); }
  Color Color::green() { COLOR(green, 0, 255, 0); }
  Color Color::blue() { COLOR(blue, 0, 0, 255); }
  Color Color::yellow() { COLOR(yellow, 255, 255, 0); }
  Color Color::magenta() { COLOR(magenta, 255, 0, 255); }
  Color Color::cyan() { COLOR(cyan, 0, 255, 255); }

}
