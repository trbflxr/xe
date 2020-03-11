//
// Created by trbflxr on 3/7/2020.
//

#ifndef XE_NODE_VIEW_INL
#define XE_NODE_VIEW_INL

namespace xe {

  template<typename T>
  void NodeView::setName(const T &value) {
    return getNode()->setName<T>(value);
  }

  template<typename T>
  void NodeView::set(const T &value) {
    getNode()->set<T>(value);
  }

  template<typename T>
  Node &NodeView::operator=(const T &rhs) {
    return *getNode() = rhs;
  }

}

#endif //XE_NODE_VIEW_INL
