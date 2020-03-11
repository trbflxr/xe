//
// Created by trbflxr on 3/7/2020.
//

#include "xepch.hpp"
#include <xe/serialization/node_const_view.hpp>

#include <xe/serialization/node.hpp>
#include <xe/utils/logger.hpp>

namespace xe {

  NodeConstView::NodeConstView(const Node *parent, Key key, const Node *value) :
      parent_(parent),
      value_(value),
      keys_{std::move(key)} {
  }

  NodeConstView::NodeConstView(const NodeConstView *parent, Key key) {
    if (!parent) {
      XE_CORE_ERROR("[NodeConstView] parent is nullptr");
      return;
    }
    parent_ = parent->parent_;
    keys_ = parent->keys_;
    keys_.emplace_back(std::move(key));
  }

  std::vector<NodeConstView> NodeConstView::getProperties(std::string_view name) const {
    if (!has_value()) {
      return { };
    }
    return value_->getProperties(name);
  }

  NodeConstView NodeConstView::getPropertyWithBackup(std::string_view name, std::string_view backupName) const {
    if (!has_value()) {
      return {this, std::string(name)};
    }
    return value_->getPropertyWithBackup(name, backupName);
  }

  NodeConstView NodeConstView::getPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const {
    if (!has_value()) {
      return {this, std::string(propertyName)};
    }
    return value_->getPropertyWithValue(propertyName, propertyValue);
  }

  NodeConstView NodeConstView::operator[](std::string_view key) const {
    if (!has_value()) {
      return {this, std::string(key)};
    }
    return value_->operator[](key);
  }

  NodeConstView NodeConstView::operator[](uint32_t index) const {
    if (!has_value()) {
      return {this, index};
    }
    return value_->operator[](index);
  }

  std::vector<Node> NodeConstView::getProperties() const {
    if (!has_value()) {
      return { };
    }
    return value_->getProperties();
  }

  std::string NodeConstView::getName() const {
    if (!has_value()) {
      return *std::get_if<std::string>(&keys_.back());
    }
    return value_->getName();
  }

  NodeConstView::Type NodeConstView::getType() const {
    if (!has_value()) {
      return Type::Unknown;
    }
    return value_->getType();
  }

}
