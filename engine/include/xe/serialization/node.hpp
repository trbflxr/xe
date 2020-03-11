//
// Created by trbflxr on 3/7/2020.
//

#ifndef XE_NODE_HPP
#define XE_NODE_HPP

#include <map>
#include <sstream>
#include <optional>
#include <xe/string.hpp>
#include <xe/serialization/node_view.hpp>

namespace xe {

  class XE_API Node {
  public:
    struct NullableChar {
      constexpr NullableChar(char val) : val(val) { }

      constexpr operator const char &() const noexcept { return val; }

      friend std::stringstream &operator<<(std::stringstream &stream, const NullableChar &c) {
        if (c.val != '\0') stream << c.val;
        return stream;
      }

      char val;
    };


    struct Format {
      constexpr Format(int8_t spacesPerIndent, char newLine, char space, bool inlineArrays) :
          spacesPerIndent(spacesPerIndent),
          newLine(newLine),
          space(space),
          inlineArrays(inlineArrays) { }

      std::string getIndents(int8_t indent) const {
        return std::string(spacesPerIndent * indent, ' ');
      }

      XE_API static const Format Beautified;
      XE_API static const Format Minified;

      int8_t spacesPerIndent;
      NullableChar newLine;
      NullableChar space;
      bool inlineArrays;
    };

    using Type = NodeConstView::Type;

    Node() = default;
    Node(const Node &node) = default;
    Node(Node &&node) = default;
    explicit Node(std::string value, Type type = Type::String);
    Node(std::string value, std::vector<Node> &&properties);

    virtual ~Node() = default;

    virtual void parseString(std::string /*string*/) { }
    virtual void writeStream(std::stringstream &/*stream*/, const Format &/*format*/) const { }

    void parseStream(std::stringstream &stream);
    std::string writeString(const Format &format = Format::Minified) const;

    template<typename T>
    T getName() const;
    template<typename T>
    void setName(const T &value);

    template<typename T>
    T get() const;

    template<typename T>
    T get(const T &fallback) const;

    template<typename T>
    bool get(T &dest) const;

    template<typename T, typename K>
    bool get(T &dest, const K &fallback) const;

    template<typename T>
    void set(const T &value);

    void clear();

    bool isValid() const;

    template<typename T>
    Node &append(const T &value);

    template<typename ...Args>
    Node &append(const Args &...args);

    bool hasProperty(std::string_view name) const;
    NodeConstView getProperty(std::string_view name) const;
    NodeConstView getProperty(uint32_t index) const;
    NodeView getProperty(std::string_view name);
    NodeView getProperty(uint32_t index);
    Node &addProperty();
    Node &addProperty(std::string_view name, Node &&node);
    Node &addProperty(std::string_view name);
    Node &addProperty(uint32_t index, Node &&node);
    Node &addProperty(uint32_t index);
    void removeProperty(std::string_view name);
    void removeProperty(const Node &node);

    std::vector<NodeConstView> getProperties(std::string_view name) const;
    NodeConstView getPropertyWithBackup(std::string_view name, std::string_view backupName) const;
    NodeConstView getPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const;
    std::vector<NodeView> getProperties(std::string_view name);
    NodeView getPropertyWithBackup(std::string_view name, std::string_view backupName);
    NodeView getPropertyWithValue(std::string_view propertyName, std::string_view propertyValue);

    NodeConstView operator[](std::string_view key) const;
    NodeConstView operator[](uint32_t index) const;
    NodeView operator[](std::string_view key);
    NodeView operator[](uint32_t index);

    Node &operator=(const Node &node) = default;
    Node &operator=(Node &&node) = default;
    template<typename T>
    Node &operator=(const T &rhs);

    bool operator==(const Node &other) const;
    bool operator!=(const Node &other) const;
    bool operator<(const Node &other) const;

    const std::vector<Node> &getProperties() const { return properties_; }
    std::vector<Node> &getProperties() { return properties_; }

    const std::string &getName() const { return name_; }
    void setName(std::string name) { name_ = std::move(name); }

    const std::string &getValue() const { return value_; }
    void setValue(std::string value) { value_ = std::move(value); }

    const Type &getType() const { return type_; }
    void setType(Type type) { type_ = type; }

  protected:
    struct Token {
      Token() = default;
      Token(Type type, std::string_view view) :
          type(type),
          view(view) {
      }

      bool operator==(const Token &other) const {
        return type == other.type && view == other.view.data();
      }

      bool operator!=(const Token &other) const {
        return !operator==(other);
      }

      Type type;
      std::string_view view;
    };

    using Tokens = std::vector<Token>;

    std::vector<Node> properties_; //members
    std::string name_; //key
    std::string value_;
    Type type_ = Type::Object;
  };

}

#include <xe/serialization/node.inl>

#endif //XE_NODE_HPP
