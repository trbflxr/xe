//
// Created by trbflxr on 3/7/2020.
//

#ifndef XE_NODE_CONST_VIEW_HPP
#define XE_NODE_CONST_VIEW_HPP

#include <variant>
#include <string>
#include <vector>
#include <xe/common.hpp>

namespace xe {

  class Node;

  class XE_API NodeConstView {
    friend class Node;
  protected:
    using Key = std::variant<std::string, uint32_t>;

    NodeConstView() = default;
    NodeConstView(const Node *parent, Key key, const Node *value);
    NodeConstView(const NodeConstView *parent, Key key);

  public:
    enum class Type : uint8_t {
      Object,
      Array,
      String,
      Boolean,
      Integer,
      Decimal,
      Null,
      Token,
      Unknown
    };

    bool has_value() const noexcept { return value_ != nullptr; }
    const Node *getNode() const { return value_; }

    explicit operator bool() const noexcept { return has_value(); }
    operator const Node &() const { return *value_; }

    const Node &operator*() const { return *value_; }
    const Node *operator->() const { return value_; }

    template<typename T>
    T getName() const;

    template<typename T>
    T get() const;

    template<typename T>
    T get(const T &fallback) const;

    template<typename T>
    bool get(T &dest) const;

    template<typename T, typename K>
    bool get(T &dest, const K &fallback) const;

    std::vector<NodeConstView> getProperties(std::string_view name) const;
    NodeConstView getPropertyWithBackup(std::string_view name, std::string_view backupName) const;
    NodeConstView getPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const;

    NodeConstView operator[](std::string_view key) const;
    NodeConstView operator[](uint32_t index) const;

    std::vector<Node> getProperties() const;

    std::string getName() const;

    Type getType() const;

  protected:
    const Node *parent_ = nullptr;
    const Node *value_ = nullptr;
    std::vector<Key> keys_;
  };

}

#include <xe/serialization/node_const_view.inl>

#endif //XE_NODE_CONST_VIEW_HPP
