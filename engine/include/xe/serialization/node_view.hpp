//
// Created by trbflxr on 3/7/2020.
//

#ifndef XE_NODE_VIEW_HPP
#define XE_NODE_VIEW_HPP

#include <xe/serialization/node_const_view.hpp>

namespace xe {

  class XE_API NodeView : public NodeConstView {
    friend class Node;
  protected:
    NodeView() = default;
    NodeView(Node *parent, Key key, Node *value);
    NodeView(NodeView *parent, Key key);

  public:
    Node *getNode();

    operator Node &() { return *getNode(); }

    Node &operator*() { return *getNode(); }
    Node *operator->() { return getNode(); }

    template<typename T>
    void setName(const T &value);

    template<typename T>
    void set(const T &value);

    std::vector<NodeView> getProperties(std::string_view name);
    NodeView getPropertyWithBackup(std::string_view name, std::string_view backupName);
    NodeView getPropertyWithValue(std::string_view propertyName, std::string_view propertyValue);

    NodeView operator[](std::string_view key);
    NodeView operator[](uint32_t index);

    template<typename T>
    Node &operator=(const T &rhs);

    std::vector<Node> &getProperties();

    void setName(const std::string &name);
  };

}

#include <xe/serialization/node_view.inl>

#endif //XE_NODE_VIEW_HPP
