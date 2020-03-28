//
// Created by trbflxr on 3/23/2020.
//

#ifndef XE_TEXT_HPP
#define XE_TEXT_HPP

#include <xe/font.hpp>

namespace xe {

  class XE_API Text : public Object {
  XE_OBJECT(Text, Object)
  public:
    Text();
    explicit Text(std::string_view string, const std::shared_ptr<Font> &font, const vec2 &position, float size);

    const std::shared_ptr<Font> &font() const { return font_; }
    void setFont(const std::shared_ptr<Font> &font) { font_ = font; }

    std::string_view string() const { return string_; }
    void setString(std::string_view string) { string_ = string; }

    float size() const { return size_; }
    void setSize(float size) { size_ = size; }

    float scale() const { return scale_; }
    void setScale(float scale) { scale_ = scale; }

    const Color &color() const { return color_; }
    void setColor(const Color &color) { color_ = color; }

    const Color &outlineColor() const { return outlineColor_; }
    void setOutlineColor(const Color &color) { outlineColor_ = color; }

    float outlineWidth() const { return outlineWidth_; }
    void setOutlineWidth(float width) { outlineWidth_ = width; }

    float outlineEdge() const { return outlineEdge_; }
    void setOutlineEdge(float edge) { outlineEdge_ = edge; }

    const vec2 &position() const { return position_; }
    void setPosition(const vec2 &position) { position_ = position; }

  private:
    std::shared_ptr<Font> font_;

    std::string string_;

    float size_ = 0.0f;
    float scale_ = 1.0f;
    Color color_ = Color::White;
    Color outlineColor_ = Color::Clear;
    float outlineWidth_ = 0.0f;
    float outlineEdge_ = 0.0f;

    vec2 position_;
  };

}

#endif //XE_TEXT_HPP
