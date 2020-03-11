//
// Created by trbflxr on 3/7/2020.
//

#ifndef XE_JSON_HPP
#define XE_JSON_HPP

#include <xe/serialization/node.hpp>

namespace xe {

  class XE_API Json : public Node {
  public:
    Json() = default;
    explicit Json(const Node &node);
    explicit Json(Node &&node);

    void parseString(std::string string) override;
    void writeStream(std::stringstream &stream, const Format &format) const override;

  private:
    static void addToken(std::string_view view, Tokens &tokens);
    static void convert(Node &current, const Tokens &tokens, int32_t i, int32_t &r);

    static void appendData(const Node &source, std::stringstream &stream, const Format &format, int8_t indent);
  };

}

#endif //XE_JSON_HPP
