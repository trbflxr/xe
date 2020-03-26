//
// Created by trbflxr on 3/23/2020.
//

#include "xepch.hpp"
#include <xe/font.hpp>

#include <xe/core/vfs.hpp>
#include <msdf-atlas-gen/msdf-atlas-gen.h>

namespace xe::detail {

  static constexpr const double DefaultEmSize = 32.0f;
  static constexpr const uint64_t McgMultiplier = 6364136223846793005;

  struct FontConfig {
    msdf_atlas::GeneratorAttributes generatorAttributes{ };
    msdf_atlas::TightAtlasPacker::DimensionsConstraint atlasSizeConstraint = msdf_atlas::TightAtlasPacker::DimensionsConstraint::MULTIPLE_OF_FOUR_SQUARE;

    int32_t width = -1;
    int32_t height = -1;
    double emSize = 32.0f;
    double pxRange = 2.0f;
    double angleThreshold = math::rad(70.0f);
    double miterLimit = 1.0f;
    uint64_t coloringSeed = 0;
    uint32_t threadCount = std::max(std::thread::hardware_concurrency(), 1u);
  };

  const uint32_t *getGlyphRangeASCII() {
    static const uint32_t ranges[]{
        0x0020, 0x00FF,
        0
    };
    return &ranges[0];
  }

  const uint32_t *getGlyphRangeCyrillic() {
    static const uint32_t ranges[]{
        0x0020, 0x00FF, //latin
        0x0400, 0x052F, //cyrillic
        0x2DE0, 0x2DFF, //cyrillic extended-A
        0xA640, 0xA69F, //cyrillic extended-B
        0
    };
    return &ranges[0];
  }

  static msdf_atlas::Charset createCharset(Charset c) {
    msdf_atlas::Charset cs;

    const uint32_t *ranges = nullptr;
    switch (c) {
      case Charset::Cyrillic: {
        ranges = getGlyphRangeCyrillic();
        break;
      }
      case Charset::Ascii:
      default: {
        ranges = getGlyphRangeASCII();
        break;
      }
    }

    for (uint32_t r = 0; r < sizeof(ranges); r += 2) {
      if (ranges[r] == 0 || ranges[r + 1] == 0) {
        break;
      }
      auto begin = ranges[r];
      auto end = ranges[r + 1];

      for (uint32_t i = begin; i < end; ++i) {
        cs.add(i);
      }
    }
    return cs;
  }

  static void loadGlyphs(std::vector<msdf_atlas::GlyphGeometry> &glyphs, msdfgen::FontHandle *font, const msdf_atlas::Charset &charset) {
    glyphs.clear();
    glyphs.reserve(charset.size());
    for (uint32_t cp : charset) {
      msdf_atlas::GlyphGeometry glyph;
      if (glyph.load(font, cp)) {
        glyphs.emplace_back(glyph);
      } else {
        XE_CORE_WARN("[Font] Missing glyph for codepoint {}", cp);
      }
    }
  }

  template<typename T, typename S, int32_t N, msdf_atlas::GeneratorFunction<S, N> GEN_FN>
  static uint8_t *makeAtlas(const std::vector<msdf_atlas::GlyphGeometry> &glyphs, const FontConfig &config) {
    msdf_atlas::ImmediateAtlasGenerator<S, N, GEN_FN, msdf_atlas::BitmapAtlasStorage<T, N>> generator(config.width, config.height);
    generator.setAttributes(config.generatorAttributes);
    generator.setThreadCount(config.threadCount);
    generator.generate(glyphs.data(), static_cast<int32_t>(glyphs.size()));
    msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>) generator.atlasStorage();

    uint32_t fontDataSize = bitmap.width * bitmap.height * 3;
    uint8_t *fd = new uint8_t[fontDataSize];
    std::memcpy(fd, bitmap.pixels, fontDataSize);
    return fd;
  }

}

namespace xe {

  bool Font::loadFromFile(std::string_view file, Charset charset) {
    auto fontData = VFS::readBytes(file);
    if (fontData.empty()) {
      return false;
    }

    return loadFromMemory(fontData.data(), fontData.size(), charset);
  }

  bool Font::loadFromMemory(const void *data, size_t size, Charset charset) {
    detail::FontConfig config{ };

    class FontHolder {
    public:
      explicit FontHolder(const void *data, size_t dataSize) :
          ft_(nullptr),
          font_(nullptr) {

        ft_ = msdfgen::initializeFreetype();
        if (ft_) {
          font_ = msdfgen::loadFont(ft_, data, dataSize);
        } else {
          XE_CORE_CRITICAL("[Font] Failed to initialize freetype");
        }
      }
      ~FontHolder() {
        if (ft_) {
          if (font_) {
            msdfgen::destroyFont(font_);
          }
          msdfgen::deinitializeFreetype(ft_);
        }
      }
      operator msdfgen::FontHandle *() const {
        return font_;
      }

    private:
      msdfgen::FreetypeHandle *ft_;
      msdfgen::FontHandle *font_;
    } font(data, size);

    if (!font) {
      XE_CORE_CRITICAL("[Font] Failed to load ft font");
      return false;
    }

    msdfgen::FontMetrics fontMetrics = { };
    msdfgen::getFontMetrics(fontMetrics, font);
    if (fontMetrics.emSize <= 0) {
      fontMetrics.emSize = detail::DefaultEmSize;
    }

    auto family = msdfgen::getFamilyName(font);
    auto style = msdfgen::getStyleName(font);

    XE_CORE_INFO("[Font] Processing font '{}' ({})", family, style);

    fontMetrics_.lineHeight = static_cast<float>(fontMetrics.lineHeight);
    fontMetrics_.ascender = static_cast<float>(fontMetrics.ascenderY);
    fontMetrics_.descender = static_cast<float>(fontMetrics.descenderY);
    fontMetrics_.underlineY = static_cast<float>(fontMetrics.underlineY);
    fontMetrics_.underlineThickness = static_cast<float>(fontMetrics.underlineThickness);

    msdf_atlas::Charset cs = detail::createCharset(charset);

    std::vector<msdf_atlas::GlyphGeometry> glyphs;
    detail::loadGlyphs(glyphs, font, cs);

    XE_CORE_INFO("[Font] Loaded geometry of {} out of {} characters", glyphs.size(), cs.size());

    //pack glyps
    msdf_atlas::TightAtlasPacker atlasPacker;
    atlasPacker.setDimensionsConstraint(config.atlasSizeConstraint);
    atlasPacker.setPadding(0);
    atlasPacker.setScale(config.emSize / fontMetrics.emSize);
    atlasPacker.setPixelRange(config.pxRange);
    atlasPacker.setUnitRange(0.0f);
    atlasPacker.setMiterLimit(config.miterLimit);

    int32_t remaining = atlasPacker.pack(glyphs.data(), static_cast<int32_t>(glyphs.size()));
    if (remaining) {
      if (remaining < 0) {
        XE_CORE_CRITICAL("[Font] Failed to pack glyphs into atlas");
        return false;
      } else {
        XE_CORE_CRITICAL("[Font] Could not fit {} out of {} glyphs into the atlas.", remaining, glyphs.size());
        return false;
      }
    }
    atlasPacker.getDimensions(config.width, config.height);
    if (!(config.width > 0 && config.height > 0)) {
      XE_CORE_CRITICAL("[Font] Unable to determine atlas size.");
      return false;
    }
    config.emSize = atlasPacker.getScale() * fontMetrics.emSize;
    config.pxRange = atlasPacker.getPixelRange();

    XE_CORE_INFO("[Font] Atlas dimensions {} x {} ", config.width, config.height);

    //edge coloring
    uint64_t glyphSeed = config.coloringSeed;
    for (msdf_atlas::GlyphGeometry &glyph : glyphs) {
      glyphSeed *= detail::McgMultiplier;
      glyph.edgeColoring(config.angleThreshold, glyphSeed);
    }

    //make texture atlas
    auto bitmapData = detail::makeAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>(glyphs, config);

    gpu::Texture::Info info;
    info.minFilter = TextureMinFilter::Linear;
    info.magFilter = TextureMagFilter::Linear;
    info.format = TexelsFormat::Rgb8;
    info.width = config.width;
    info.height = config.height;
    info.bindless = true;

    atlas_ = std::make_shared<Texture>();
    atlas_->setInfo(info);
    atlas_->setData(bitmapData, 0);
    atlas_->setup();

    //fill glyphs
    double scale = 1.0f / fontMetrics.emSize;
    double l = 0.0f;
    double b = 0.0f;
    double r = 0.0f;
    double t = 0.0f;

    glyphs_.reserve(glyphs.size());
    for (const auto &g : glyphs) {
      Glyph glyph{ };
      glyph.unicode = g.getCodepoint();
      glyph.advance = static_cast<float>(scale * g.getAdvance());

      g.getQuadPlaneBounds(l, b, r, t);
      glyph.planeBounds.left = static_cast<float>(scale * l);
      glyph.planeBounds.bottom = static_cast<float>(scale * b);
      glyph.planeBounds.right = static_cast<float>(scale * r);
      glyph.planeBounds.top = static_cast<float>(scale * t);

      g.getQuadAtlasBounds(l, b, r, t);
      glyph.atlasBounds.left = static_cast<float>(l / config.width);
      glyph.atlasBounds.bottom = static_cast<float>(b / config.height);
      glyph.atlasBounds.right = static_cast<float>(r / config.width);
      glyph.atlasBounds.top = static_cast<float>(t / config.height);

      glyphs_.emplace_back(glyph);
    }

    //kerning
    for (size_t i = 0; i < glyphs.size(); ++i) {
      for (size_t j = 0; j < glyphs.size(); ++j) {
        double kerning = 0.0f;
        if (msdfgen::getKerning(kerning, font, glyphs[i].getCodepoint(), glyphs[j].getCodepoint()) && kerning) {
          kerning_.push_back({glyphs[i].getCodepoint(), glyphs[j].getCodepoint(), static_cast<float>(scale * kerning)});
        }
      }
    }

    return true;
  }

  const Glyph &Font::getGlyph(uint32_t codePoint) const {
    for (const auto &glyph : glyphs_) {
      if (glyph.unicode == codePoint) {
        return glyph;
      }
    }
    XE_CORE_CRITICAL("[Font] Unable to find glyph cp: {}", codePoint);
    static const Glyph g{ };
    return g;
  }

  float Font::getKerning(uint32_t first, uint32_t second) const {
    for (const auto &k : kerning_) {
      if (k.cp0 == first && k.cp1 == second) {
        return k.advance;
      }
    }
    return 0.0f;
  }

}
