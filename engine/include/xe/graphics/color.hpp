//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_COLOR_HPP
#define XE_COLOR_HPP

#include <xe/math/math.hpp>

namespace xe {

  struct XE_API Color {
    float r, g, b, a;

    Color();
    Color(const Color &o);
    explicit Color(const vec4 &rgba);
    Color(float r, float g, float b, float a);
    explicit Color(uint hex);

    inline vec4 rgba() const { return vec4(r, g, b, a); }
    inline vec3 rgb() const { return vec3(r, g, b); }
    uint hex() const;

    float luminance() const;
    Color desaturate(float factor) const;

    static Color lerp(const Color &a, const Color &b, float alpha);

    static Color Transparent();
    static Color Black();
    static Color White();
    static Color Red();
    static Color Green();
    static Color Blue();
    static Color Yellow();
    static Color Magenta();
    static Color Cyan();

    inline Color operator+(const Color& o) const { return Color(r + o.r, g + o.g, b + o.b, a + o.a); }
    inline Color& operator+=(const Color& o) { r += o.r; g += o.g; b += o.b; a += o.a; return *this; }

    inline Color operator-(const Color& o) const { return Color(r - o.r, g - o.g, b - o.b, a - o.a); }
    inline Color& operator-=(const Color& o) { r -= o.r; g -= o.g; b -= o.b; a -= o.a; return *this; }

    inline Color operator*(const Color& o) const { return Color(r * o.r, g * o.g, b * o.b, a * o.a); }
    inline Color& operator*=(const Color& o) { r *= o.r; g *= o.g; b *= o.b; a *= o.a; return *this; }

    inline Color operator*(float value) const { return Color(r * value, g * value, b * value, a * value); }
    inline Color& operator*=(float value) { r *= value; g *= value; b *= value; a *= value; return *this; }

    inline Color operator/(const Color& o) const { return Color(r / o.r, g / o.g, b / o.b, a / o.a); }
    inline Color& operator/=(const Color& o) { r /= o.r; g /= o.g; b /= o.b; a /= o.a; return *this; }

    inline Color operator/(float value) const { return Color(r / value, g / value, b / value, a / value); }
    inline Color& operator/=(float value) { r /= value; g /= value; b /= value; a /= value; return *this; }

    inline bool operator==(const Color& o) const { return r == o.r && g == o.g && b == o.b && a == o.a; }
    inline bool operator!=(const Color& o) const { return r != o.r || g != o.g || b != o.b || a != o.a; }
  };

}

#endif //XE_COLOR_HPP
