//
// Created by trbflxr on 3/7/2020.
//

#ifndef XE_NODE_INL
#define XE_NODE_INL

namespace xe {

  template<typename T>
  T Node::getName() const {
    return string::from<T>(name_);
  }

  template<typename T>
  void Node::setName(const T &value) {
    name_ = string::to<T>(value);
  }

  template<typename T>
  T Node::get() const {
    T value;
    *this >> value;
    return value;
  }

  template<typename T>
  T Node::get(const T &fallback) const {
    if (!isValid()) {
      return fallback;
    }
    return get < T > ();
  }

  template<typename T>
  bool Node::get(T &dest) const {
    if (!isValid()) {
      return false;
    }
    *this >> dest;
    return true;
  }

  template<typename T, typename K>
  bool Node::get(T &dest, const K &fallback) const {
    if (!isValid()) {
      dest = fallback;
      return false;
    }
    *this >> dest;
    return true;
  }

  template<typename T>
  void Node::set(const T &value) {
    *this << value;
  }

  template<typename T>
  Node &Node::append(const T &value) {
    addProperty() << value;
    return *this;
  }

  template<typename ...Args>
  Node &Node::append(const Args &...args) {
    (append(args), ...);
    return *this;
  }

  template<typename T>
  Node &Node::operator=(const T &rhs) {
    set(rhs);
    return *this;
  }

  inline const Node &operator>>(const Node &node, Node &object) {
    object = node;
    return node;
  }

  inline Node &operator<<(Node &node, const Node &object) {
    node = object;
    return node;
  }

  inline Node &operator<<(Node &node, const std::nullptr_t &/*object*/) {
    node.setValue("");
    node.setType(Node::Type::Null);
    return node;
  }

  template<typename T, std::enable_if_t<std::is_pointer_v<T>, int32_t> = 0>
  Node &operator<<(Node &node, const T object) {
    if (object == nullptr) {
      return node << nullptr;
    }

    node << *object;
    return node;
  }

  template<typename T>
  const Node &operator>>(const Node &node, std::unique_ptr<T> &object) {
    object = std::make_unique<T>();
    node >> *object;
    return node;
  }

  template<typename T>
  Node &operator<<(Node &node, const std::unique_ptr<T> &object) {
    if (object == nullptr) {
      return node << nullptr;
    }

    node << *object;
    return node;
  }

  template<typename T>
  const Node &operator>>(const Node &node, std::shared_ptr<T> &object) {
    if constexpr (std::is_base_of_v<Resource, T>) {
      object = T::create(node);
      return node;
    } else {
      object = std::make_shared<T>();
      node >> *object;
      return node;
    }
  }

  template<typename T>
  Node &operator<<(Node &node, const std::shared_ptr<T> &object) {
    if (object == nullptr) {
      return node << nullptr;
    }
    node << *object;
    return node;
  }

  inline const Node &operator>>(const Node &node, bool &object) {
    object = string::from<bool>(node.getValue());
    return node;
  }

  inline Node &operator<<(Node &node, bool object) {
    node.setValue(string::to(object));
    node.setType(Node::Type::Boolean);
    return node;
  }

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, int32_t> = 0>
  const Node &operator>>(const Node &node, T &object) {
    object = string::from<T>(node.getValue());
    return node;
  }

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, int32_t> = 0>
  Node &operator<<(Node &node, T object) {
    node.setValue(string::to(object));
    node.setType(std::is_floating_point_v<T> ? Node::Type::Decimal : Node::Type::Integer);
    return node;
  }

  inline const Node &operator>>(const Node &node, char *&string) {
    strcpy_s(string, node.getValue().size(), node.getValue().c_str());
    return node;
  }

  inline Node &operator<<(Node &node, const char *string) {
    node.setValue(string);
    node.setType(Node::Type::String);
    return node;
  }

  inline Node &operator<<(Node &node, std::string_view string) {
    node.setValue(string.data());
    node.setType(Node::Type::String);
    return node;
  }

  inline const Node &operator>>(const Node &node, std::string &string) {
    string = node.getValue();
    return node;
  }

  inline Node &operator<<(Node &node, const std::string &string) {
    node.setValue(string);
    node.setType(Node::Type::String);
    return node;
  }

  template<typename T, typename K>
  const Node &operator>>(const Node &node, std::pair<T, K> &pair) {
    pair.first = node.getName<T>();
    node >> pair.second;
    return node;
  }

  template<typename T, typename K>
  Node &operator<<(Node &node, const std::pair<T, K> &pair) {
    node.setName(string::to(pair.first));
    node << pair.second;
    return node;
  }

  template<typename T>
  const Node &operator>>(const Node &node, std::optional<T> &optional) {
    if (node.getValue() != "null") {
      T x;
      node >> x;
      optional = std::move(x);
    } else {
      optional = std::nullopt;
    }
    return node;
  }

  template<typename T>
  Node &operator<<(Node &node, const std::optional<T> &optional) {
    if (optional) {
      return node << *optional;
    }
    return node << nullptr;
  }

  template<typename T>
  const Node &operator>>(const Node &node, std::vector<T> &vector) {
    vector.clear();
    vector.reserve(node.getProperties().size());

    for (const auto &property : node.getProperties()) {
      T x;
      property >> x;
      vector.emplace_back(std::move(x));
    }
    return node;
  }

  template<typename T>
  Node &operator<<(Node &node, const std::vector<T> &vector) {
    for (const auto &x : vector) {
      node.addProperty() << x;
    }

    node.setType(Node::Type::Array);
    return node;
  }

  template<typename T, typename K>
  const Node &operator>>(const Node &node, std::map<T, K> &map) {
    map.clear();

    for (const auto &property : node.getProperties()) {
      std::pair<T, K> pair;
      property >> pair;
      map.emplace(std::move(pair));
    }
    return node;
  }

  template<typename T, typename K>
  Node &operator<<(Node &node, const std::map<T, K> &map) {
    for (const auto &x : map) {
      node.addProperty() << x;
    }

    node.setType(Node::Type::Array);
    return node;
  }

}

#endif //XE_NODE_INL
