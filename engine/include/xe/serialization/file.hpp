//
// Created by trbflxr on 3/7/2020.
//

#ifndef XE_FILE_HPP
#define XE_FILE_HPP

#include <xe/serialization/node.hpp>

namespace xe {

  class XE_API File {
  public:
    File() = default;
    explicit File(std::unique_ptr<Node> &&node);
    explicit File(std::string_view file);
    File(std::string_view file, std::unique_ptr<Node> &&node);

    bool load(std::string_view file);
    bool load();

    bool write(std::string_view file, Node::Format format = Node::Format::Minified) const;
    bool write(Node::Format format = Node::Format::Minified) const;

    void clear();

    Node *getNode() const { return node_.get(); }

    std::string_view getFilename() const { return filename_; }
    void setFilename(std::string_view file) { filename_ = file; }

  private:
    std::unique_ptr<Node> node_;
    std::string filename_;
  };

}

#endif //XE_FILE_HPP
