//
// Created by trbflxr on 3/7/2020.
//

#include "xepch.hpp"
#include <xe/serialization/file.hpp>

#include <xe/core/vfs.hpp>
#include <xe/serialization/json.hpp>
#include <xe/utils/debug_trace.hpp>

namespace xe {

  File::File(std::unique_ptr<Node> &&node) :
      node_(std::move(node)) {
  }

  File::File(std::string_view file) :
      filename_(file) {

    auto ext = string::getFileExt(filename_);

    if (ext == "json") {
      node_ = std::make_unique<Json>();
    } else {
      XE_CORE_CRITICAL("[File] Unsupported serialization format");
      throw std::runtime_error("Unsupported serialization format");
    }
  }

  File::File(std::string_view file, std::unique_ptr<Node> &&node) :
      node_(std::move(node)),
      filename_(file) {
  }


  bool File::load(std::string_view file) {
#if XE_DEBUG_TRACING
    const std::string name = std::string("Read serialization file ") + file.data();
#else
    static const std::string name("Read serialization file");
#endif

    XE_TRACE_BEGIN("XE", name.c_str());

    auto str = VFS::readText(file);
    if (!str) {
      XE_CORE_ERROR("[File] Unable to read serialization file '{}'", file);
      return false;
    }

    node_->parseString(str.value());

    XE_TRACE_END("XE", name.c_str());
    return true;
  }

  bool File::load() {
    return load(filename_);
  }

  bool File::write(std::string_view file, Node::Format format) const {
#if XE_DEBUG_TRACING
    const std::string name = std::string("Write serialization file ") + file.data();
#else
    static const std::string name("Write serialization file");
#endif

    XE_TRACE_BEGIN("XE", name.c_str());

    auto lastSlash = file.find_last_of('/');
    if (lastSlash != std::string::npos) {
      std::string dir(file.substr(0, lastSlash));
      if (!xe::VFS::mkdir(dir)) {
        XE_CORE_ERROR("[File] Failed to make directory '{}'.", dir);
        XE_TRACE_END("XE", name.c_str());
        return false;
      }
    }

    std::stringstream ss;
    node_->writeStream(ss, format);

    if (!VFS::write(file, ss.str().data(), ss.str().size())) {
      XE_CORE_ERROR("[File] Failed to write png to '{}'.", file);
      XE_TRACE_END("XE", name.c_str());
      return false;
    }

    XE_TRACE_END("XE", name.c_str());
    return true;
  }

  bool File::write(Node::Format format) const {
    return write(filename_, format);
  }

  void File::clear() {
    node_->clear();
  }

}
