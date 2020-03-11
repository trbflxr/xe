//
// Created by trbflxr on 3/7/2020.
//

#include "xepch.hpp"
#include <xe/serialization/node.hpp>

#include <algorithm>

namespace xe {

  const Node::Format Node::Format::Beautified = Format(2, '\n', ' ', true);
  const Node::Format Node::Format::Minified = Format(0, '\0', '\0', false);

  static const Node NullNode = Node("null", Node::Type::Null);

  Node::Node(std::string value, Type type) :
      value_(std::move(value)),
      type_(type) {
  }

  Node::Node(std::string value, std::vector<Node> &&properties) :
      properties_(std::move(properties)),
      value_(std::move(value)) {
  }

  void Node::parseStream(std::stringstream &stream) {
    std::string s = stream.str();
    parseString(s);
  }

  std::string Node::writeString(const Format &format) const {
    std::stringstream stream;
    writeStream(stream, format);
    return stream.str();
  }

  void Node::clear() {
    properties_.clear();
  }

  bool Node::isValid() const {
    switch (type_) {
      case Type::Token:
      case Type::Unknown:return false;
      case Type::Object:
      case Type::Array:return !properties_.empty();
      case Type::Null:return true;
      default:return !value_.empty();
    }
  }

  bool Node::hasProperty(std::string_view name) const {
    for (const auto &property : properties_) {
      if (property.name_ == name) {
        return true;
      }
    }
    return false;
  }

  NodeConstView Node::getProperty(std::string_view name) const {
    for (const auto &property : properties_) {
      if (property.name_ == name) {
        return {this, std::string(name), &property};
      }
    }
    return {this, std::string(name), nullptr};
  }

  NodeConstView Node::getProperty(uint32_t index) const {
    if (index < properties_.size()) {
      return {this, index, &properties_[index]};
    }
    return {this, index, nullptr};
  }

  NodeView Node::getProperty(std::string_view name) {
    for (auto &property : properties_) {
      if (property.name_ == name) {
        return {this, std::string(name), &property};
      }
    }
    return {this, std::string(name), nullptr};
  }

  NodeView Node::getProperty(uint32_t index) {
    if (index < properties_.size()) {
      return {this, index, &properties_[index]};
    }
    return {this, index, nullptr};
  }

  Node &Node::addProperty() {
    return properties_.emplace_back();
  }

  Node &Node::addProperty(std::string_view name, Node &&node) {
    node.name_ = name;
    return properties_.emplace_back(std::move(node));
  }

  Node &Node::addProperty(std::string_view name) {
    Node node;
    node.name_ = name;
    return properties_.emplace_back(std::move(node));
  }

  Node &Node::addProperty(uint32_t index, Node &&node) {
    properties_.resize(std::max(properties_.size(), static_cast<size_t>(index + 1)), NullNode);
    return properties_[index] = std::move(node);
  }

  Node &Node::addProperty(uint32_t index) {
    properties_.resize(std::max(properties_.size(), static_cast<size_t>(index + 1)), NullNode);
    return properties_[index];
  }

  void Node::removeProperty(std::string_view name) {
    properties_.erase(std::remove_if(properties_.begin(), properties_.end(), [name](const auto &n) {
      return n.getName() == name;
    }), properties_.end());
  }

  void Node::removeProperty(const Node &node) {
    properties_.erase(std::remove_if(properties_.begin(), properties_.end(), [node](const auto &n) {
      return n == node;
    }), properties_.end());
  }

  std::vector<NodeConstView> Node::getProperties(std::string_view name) const {
    std::vector<NodeConstView> properties;

    for (const auto &property : properties_) {
      if (property.name_ == name) {
        properties.emplace_back(NodeConstView(this, std::string(name), &property));
      }
    }
    return properties;
  }

  NodeConstView Node::getPropertyWithBackup(std::string_view name, std::string_view backupName) const {
    if (auto p1 = getProperty(name)) return p1;
    if (auto p2 = getProperty(backupName)) return p2;

    return {this, std::string(name), nullptr};
  }

  NodeConstView Node::getPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const {
    for (const auto &property : properties_) {
      auto properties1 = property.getProperties(propertyName);
      if (properties1.empty()) {
        return {this, std::string(propertyName), nullptr};
      }

      for (auto &property1 : properties1) {
        if (property1 && property1->getValue() == propertyValue) {
          return {this, std::string(propertyName), &property};
        }
      }
    }
    return {this, std::string(propertyName), nullptr};
  }

  std::vector<NodeView> Node::getProperties(std::string_view name) {
    std::vector<NodeView> properties;

    for (auto &property : properties_) {
      if (property.name_ == name) {
        properties.emplace_back(NodeView(this, std::string(name), &property));
      }
    }
    return properties;
  }

  NodeView Node::getPropertyWithBackup(std::string_view name, std::string_view backupName) {
    if (auto p1 = getProperty(name)) return p1;
    if (auto p2 = getProperty(backupName)) return p2;

    return {this, std::string(name), nullptr};
  }

  NodeView Node::getPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) {
    for (auto &property : properties_) {
      auto properties1 = property.getProperties(propertyName);
      if (properties1.empty()) {
        return {this, std::string(propertyName), nullptr};
      }

      for (auto &property1 : properties1) {
        if (property1 && property1->getValue() == propertyValue) {
          return {this, std::string(propertyName), &property};
        }
      }
    }
    return {this, std::string(propertyName), nullptr};
  }

  NodeConstView Node::operator[](std::string_view key) const {
    return getProperty(key);
  }

  NodeConstView Node::operator[](uint32_t index) const {
    return getProperty(index);
  }

  NodeView Node::operator[](std::string_view key) {
    return getProperty(key);
  }

  NodeView Node::operator[](uint32_t index) {
    return getProperty(index);
  }

  bool Node::operator==(const Node &other) const {
    return value_ == other.value_ && properties_.size() == other.properties_.size() &&
           std::equal(properties_.begin(), properties_.end(), other.properties_.begin(), [](const auto &left, const auto &right) {
             return left == right;
           });
  }

  bool Node::operator!=(const Node &other) const {
    return !operator==(other);
  }

  bool Node::operator<(const Node &other) const {
    if (name_ < other.name_) return true;
    if (other.name_ < name_) return false;

    if (value_ < other.value_) return true;
    if (other.value_ < value_) return false;

    if (properties_ < other.properties_) return true;
    if (other.properties_ < properties_) return false;

    return false;
  }

}
