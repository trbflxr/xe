//
// Created by trbflxr on 3/7/2020.
//

#include "xepch.hpp"
#include <xe/serialization/json.hpp>

namespace xe {

  Json::Json(const Node &node) :
      Node(node) {
    setType(Type::Object);
  }

  Json::Json(Node &&node) :
      Node(std::move(node)) {
    setType(Type::Object);
  }

  void Json::parseString(std::string string) {
    Tokens tokens;

    size_t tokenStart = 0;
    enum class QuoteState : char {
      None = '\0',
      Single = '\'',
      Double = '"'
    } quoteState = QuoteState::None;

    //iterates over all the characters in the string view
    for (size_t index = 0; index < string.length(); ++index) {
      auto c = string[index];
      //if previous character was a backslash quote will not break the string
      if (c == '\'' && quoteState != QuoteState::Double && string[index - 1] != '\\') {
        quoteState = quoteState == QuoteState::None ? QuoteState::Single : QuoteState::None;
      } else if (c == '"' && quoteState != QuoteState::Single && string[index - 1] != '\\') {
        quoteState = quoteState == QuoteState::None ? QuoteState::Double : QuoteState::None;
      }

      //when not reading a string tokens can be found
      //while in a string whitespace and tokens are added to the strings view
      if (quoteState == QuoteState::None) {
        if (string::isWhitespace(c)) {
          //on whitespace start save current token
          addToken(std::string_view(string.data() + tokenStart, index - tokenStart), tokens);
          tokenStart = index + 1;
        } else if (c == ':' || c == '{' || c == '}' || c == ',' || c == '[' || c == ']') {
          //tokens used to read json nodes
          addToken(std::string_view(string.data() + tokenStart, index - tokenStart), tokens);
          tokens.emplace_back(Type::Token, std::string_view(string.data() + index, 1));
          tokenStart = index + 1;
        }
      }
    }

    if (tokens.empty()) {
      throw std::runtime_error("No tokens found in document");
    }

    //convert the tokens into nodes
    int32_t k = 0;
    convert(*this, tokens, 0, k);
  }

  void Json::writeStream(std::stringstream &stream, const Node::Format &format) const {
    stream << (getType() == Type::Array ? '[' : '{') << format.newLine;
    appendData(*this, stream, format, 1);
    stream << (getType() == Type::Array ? ']' : '}');
  }

  void Json::addToken(std::string_view view, Node::Tokens &tokens) {
    if (view.length() != 0) {
      //find node value type of the string and adds it to tokens
      if (view == "null") {
        tokens.emplace_back(Type::Null, std::string_view());
      } else if (view == "true" || view == "false") {
        tokens.emplace_back(Type::Boolean, view);
      } else if (string::isNumber(view)) {
        //if the number is a decimal
        if (view.find('.') != std::string::npos) {
          if (view.size() >= std::numeric_limits<long double>::digits) {
            throw std::runtime_error("Decimal number is too long");
          }
          tokens.emplace_back(Type::Decimal, view);
        } else {
          if (view.size() >= std::numeric_limits<uint64_t>::digits) {
            throw std::runtime_error("Integer number is too long");
          }
          tokens.emplace_back(Type::Integer, view);
        }
      } else {
        tokens.emplace_back(Type::String, view.substr(1, view.length() - 2));
      }
    }
  }

  void Json::convert(Node &current, const Node::Tokens &tokens, int32_t i, int32_t &r) {
    if (tokens[i] == Token(Type::Token, "{")) {
      auto k = i + 1;

      while (tokens[k] != Token(Type::Token, "}")) {
        auto key = tokens[k].view;
        if (k + 2 >= tokens.size())
          throw std::runtime_error("Missing end of {} array");
        if (tokens[k + 1].view != ":")
          throw std::runtime_error("Missing object colon");
        k += 2;
        convert(current.addProperty(key), tokens, k, k);
        if (tokens[k].view == ",")
          k++;
      }

      current.setType(Type::Object);
      r = k + 1;
    } else if (tokens[i] == Token(Type::Token, "[")) {
      auto k = i + 1;

      while (tokens[k] != Token(Type::Token, "]")) {
        if (k >= tokens.size()) {
          throw std::runtime_error("Missing end of [] object");
        }
        convert(current.addProperty(), tokens, k, k);
        if (tokens[k].view == ",") {
          k++;
        }
      }

      current.setType(Type::Array);
      r = k + 1;
    } else {
      std::string str(tokens[i].view);
      if (tokens[i].type == Type::String) {
        str = string::unfixEscapedChars(str);
      }
      current.setValue(str);
      current.setType(tokens[i].type);
      r = i + 1;
    }
  }

  void Json::appendData(const Node &source, std::stringstream &stream, const Node::Format &format, int8_t indent) {
    auto indents = format.getIndents(indent);

    //only output value if no properties exist
    if (source.getProperties().empty()) {
      if (source.getType() == Type::String) {
        stream << '\"' << string::fixEscapedChars(source.getValue()) << '\"';
      } else if (source.getType() == Type::Null) {
        stream << "null";
      } else {
        stream << source.getValue();
      }
    }

    //output each property
    for (auto it = source.getProperties().begin(); it < source.getProperties().end(); ++it) {
      stream << indents;
      //output name for property if it exists
      if (!it->getName().empty()) {
        stream << '\"' << it->getName() << "\":" << format.space;
      }

      bool isArray = false;
      if (!it->getProperties().empty()) {
        //this must be an array.
        for (const auto &property2 : it->getProperties()) {
          if (property2.getName().empty()) {
            isArray = true;
            break;
          }
        }

        stream << (isArray ? '[' : '{') << format.newLine;
      } else if (it->getType() == Type::Object) {
        stream << '{';
      } else if (it->getType() == Type::Array) {
        stream << '[';
      }

      //if node type is a primitive type
      static constexpr auto IsPrimitive = [](Type type) {
        return type != Type::Object && type != Type::Array && type != Type::Unknown;
      };

      //shorten primitive array output length
      if (isArray && format.inlineArrays && !it->getProperties().empty() && IsPrimitive(it->getProperties()[0].getType())) {
        stream << format.getIndents(indent + 1);
        //new lines are printed space
        appendData(*it, stream, Format(0, ' ', '\0', false), indent);
        stream << '\n';
      } else {
        appendData(*it, stream, format, indent + 1);
      }

      if (!it->getProperties().empty()) {
        stream << indents << (isArray ? ']' : '}');
      } else if (it->getType() == Type::Object) {
        stream << '}';
      } else if (it->getType() == Type::Array) {
        stream << ']';
      }

      //separate properties by comma
      if (it != source.getProperties().end() - 1) {
        stream << ',';
      }
      //no new line if the indent level is zero
      stream << (indent != 0 ? format.newLine : format.space);
    }
  }

}
