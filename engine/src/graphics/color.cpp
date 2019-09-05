//
// Created by FLXR on 9/3/2019.
//

#include <xe/graphics/color.hpp>

namespace xe {

  Color::Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) { }
  Color::Color(const Color &o) : r(o.r), g(o.g), b(o.b), a(o.a) { }
  Color::Color(const vec4 &rgba) : r(rgba[0]), g(rgba[1]), b(rgba[2]), a(rgba[3]) { }
  Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
  Color::Color(uint hex) {
    b = ((hex >> 0) & 255) / 255.0f;
    g = ((hex >> 8) & 255) / 255.0f;
    r = ((hex >> 16) & 255) / 255.0f;
    a = ((hex >> 24) & 255) / 255.0f;
  }

  uint Color::hex() const {
    const uint r1 = static_cast<uint>(r * 255);
    const uint g1 = static_cast<uint>(g * 255);
    const uint b1 = static_cast<uint>(b * 255);
    const uint a1 = static_cast<uint>(a * 255);

    const uint result = a1 << 24 | r1 << 16 | g1 << 8 | b1;
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

  Color Color::Transparent() {
    static Color transparent = Color(0.0f, 0.0f, 0.0f, 0.0f);
    return transparent;
  }

  Color Color::Black() { COLOR(Black, 0, 0, 0); }
  Color Color::White() { COLOR(White, 255, 255, 255); }
  Color Color::Red() { COLOR(Red, 255, 0, 0); }
  Color Color::Green() { COLOR(Green, 0, 255, 0); }
  Color Color::Blue() { COLOR(Blue, 0, 0, 255); }
  Color Color::Yellow() { COLOR(Yellow, 255, 255, 0); }
  Color Color::Magenta() { COLOR(Magenta, 255, 0, 255); }
  Color Color::Cyan() { COLOR(Cyan, 0, 255, 255); }

}
