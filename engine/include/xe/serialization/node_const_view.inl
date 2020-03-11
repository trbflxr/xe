//
// Created by trbflxr on 3/7/2020.
//

#ifndef XE_NODE_CONST_VIEW_INL
#define XE_NODE_CONST_VIEW_INL

namespace xe {

  template<typename T>
  T NodeConstView::getName() const {
    if (!has_value()) {
      return { };
    }
    return value_->getName<T>();
  }

  template<typename T>
  T NodeConstView::get() const {
    if (!has_value()) {
      return { };
    }
    return value_->get<T>();
  }

  template<typename T>
  T NodeConstView::get(const T &fallback) const {
    if (!has_value()) {
      return fallback;
    }
    return value_->get<T>(fallback);
  }

  template<typename T>
  bool NodeConstView::get(T &dest) const {
    if (!has_value()) {
      return false;
    }
    return value_->get<T>(dest);
  }

  template<typename T, typename K>
  bool NodeConstView::get(T &dest, const K &fallback) const {
    if (!has_value()) {
      dest = fallback;
      return false;
    }
    return value_->get<T>(dest, fallback);
  }

}

#endif //XE_NODE_CONST_VIEW_INL
