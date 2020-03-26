//
// Created by trbflxr on 3/23/2020.
//

#ifndef XE_FONT_HPP
#define XE_FONT_HPP

#include <xe/common.hpp>
#include <xe/graphics/texture.hpp>
#include <xe/math/rect.hpp>

namespace xe {

  struct XE_API Glyph {
    uint32_t unicode;
    float advance;

    struct {
      float left;
      float bottom;
      float right;
      float top;
    } planeBounds;

    struct {
      float left;
      float bottom;
      float right;
      float top;
    } atlasBounds;
  };

  struct XE_API FontMetrics {
    float lineHeight;
    float ascender;
    float descender;
    float underlineY;
    float underlineThickness;
  };

  enum class Charset {
    Ascii,
    Cyrillic
  };

  class XE_API Font : public Object {
  XE_OBJECT(Font, Object)
  public:
    Font() = default;

    bool loadFromFile(std::string_view file, Charset charset = Charset::Ascii);
    bool loadFromMemory(const void *data, size_t size, Charset charset = Charset::Ascii);

    const Glyph &getGlyph(uint32_t codePoint) const;

    const FontMetrics &fontMetrics() const { return fontMetrics_; }
    float getKerning(uint32_t first, uint32_t second) const;

    const std::shared_ptr<Texture> &atlas() const { return atlas_; }

  private:
    struct Kerning {
      uint32_t cp0;
      uint32_t cp1;
      float advance;
    };

    std::shared_ptr<Texture> atlas_;
    std::vector<Glyph> glyphs_;
    std::vector<Kerning> kerning_;
    FontMetrics fontMetrics_;
  };

}

#endif //XE_FONT_HPP
