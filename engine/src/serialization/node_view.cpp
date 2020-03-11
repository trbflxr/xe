//
// Created by trbflxr on 3/7/2020.
//

#include "xepch.hpp"
#include <xe/serialization/node_view.hpp>

#include <xe/serialization/node.hpp>
#include <xe/utils/logger.hpp>

namespace xe {

  NodeView::NodeView(Node *parent, NodeConstView::Key key, Node *value) :
      NodeConstView(parent, std::move(key), value) {
  }

  NodeView::NodeView(NodeView *parent, NodeConstView::Key key) :
      NodeConstView(parent, std::move(key)) {
  }

  Node *NodeView::getNode() {
    if (!has_value()) {
      for (const auto &key : keys_) {
        if (const auto name = std::get_if<std::string>(&key)) {
          value_ = &const_cast<Node *>(parent_)->addProperty(*name, { });
        } else if (const auto index = std::get_if<std::uint32_t>(&key)) {
          value_ = &const_cast<Node *>(parent_)->addProperty(*index, { });
        } else {
          XE_CORE_CRITICAL("[NodeView] Key for node return is neither a int or a string");
          throw std::runtime_error("Key for node return is neither a int or a string");
        }
        parent_ = value_;
      }
      keys_.erase(keys_.begin(), keys_.end() - 1);
    }
    return const_cast<Node *>(value_);
  }

  std::vector<NodeView> NodeView::getProperties(std::string_view name) {
    if (!has_value()) {
      return { };
    }
    return const_cast<Node *>(value_)->getProperties(name);
  }

  NodeView NodeView::getPropertyWithBackup(std::string_view name, std::string_view backupName) {
    if (!has_value()) {
      return {this, std::string(name)};
    }
    return const_cast<Node *>(value_)->getPropertyWithBackup(name, backupName);
  }

  NodeView NodeView::getPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) {
    if (!has_value()) {
      return {this, std::string(propertyName)};
    }
    return const_cast<Node *>(value_)->getPropertyWithValue(propertyName, propertyValue);
  }

  NodeView NodeView::operator[](std::string_view key) {
    if (!has_value()) {
      return {this, std::string(key)};
    }
    return const_cast<Node *>(value_)->operator[](key);
  }

  NodeView NodeView::operator[](uint32_t index) {
    if (!has_value()) {
      return {this, index};
    }
    return const_cast<Node *>(value_)->operator[](index);
  }

  std::vector<Node> &NodeView::getProperties() {
    if (!has_value()) {
      return getNode()->getProperties();
    }
    return const_cast<Node *>(value_)->getProperties();
  }

  void NodeView::setName(const std::string &name) {
    if (!has_value()) {
      keys_.back() = name.data();
      return;
    }
    const_cast<Node *>(value_)->setName(name);
  }

}
