//
// Created by trbflxr on 3/30/2020.
//

#include "xepch.hpp"
#include "assimp_io.hpp"

#include <assimp/StringComparison.h>

using namespace xe;

namespace Assimp {

  ///XEAI_IOSystem
  bool XEAI_IOSystem::Exists(const char *file) const {
    return VFS::exists(file);
  }

  char XEAI_IOSystem::getOsSeparator() const {
    return '/';
  }

  IOStream *XEAI_IOSystem::Open(const char *file, const char *mode) {
    VFS::OpenMode m = VFS::OpenMode::Read;
    if (strcmp(mode, "wt") == 0 || strcmp(mode, "wb") == 0) {
      m = VFS::OpenMode::Write;
    }

    VFS::File fsFile = VFS::open(file, m);
    if (!fsFile.handle) {
      return nullptr;
    }
    return new XEAI_IOStream(std::move(fsFile));
  }

  void XEAI_IOSystem::Close(IOStream *file) {
    delete file;
  }

  bool XEAI_IOSystem::ComparePaths(const char *one, const char *second) const {
    return !ASSIMP_stricmp(one, second);
  }

  ///XEAI_IOStream
  XEAI_IOStream::XEAI_IOStream() noexcept :
      cachedSize_(SIZE_MAX) { }

  XEAI_IOStream::XEAI_IOStream(xe::VFS::File &&handle) :
      file_(std::move(handle)),
      cachedSize_(SIZE_MAX) { }

  XEAI_IOStream::~XEAI_IOStream() {
    if (file_.handle) {
      if (VFS::close(file_)) {
        file_.handle = nullptr;
      }
    }
  }

  size_t XEAI_IOStream::Read(void *buffer, size_t size, size_t count) {
    if (file_.handle) {
      if (VFS::read(file_, buffer, size * count)) {
        return count;
      }
    }
    return 0;
  }

  size_t XEAI_IOStream::Write(const void *buffer, size_t size, size_t count) {
    if (file_.handle) {
      if (VFS::write(file_, buffer, size * count)) {
        return count;
      }
    }
    return 0;
  }

  aiReturn XEAI_IOStream::Seek(size_t offset, aiOrigin origin) {
    if (!file_.handle) {
      return aiReturn_FAILURE;
    }

    VFS::Seek o = VFS::Seek::Set;
    if (origin == aiOrigin_CUR) {
      o = VFS::Seek::Cur;
    } else if (origin == aiOrigin_END) {
      o = VFS::Seek::End;
    }

    return VFS::seek(file_, o, offset) ? aiReturn_SUCCESS : aiReturn_FAILURE;
  }

  size_t XEAI_IOStream::Tell() const {
    if (!file_.handle) {
      return 0;
    }
    return VFS::tell(file_);
  }

  size_t XEAI_IOStream::FileSize() const {
    if (!file_.handle || file_.path.empty()) {
      return 0;
    }
    if (cachedSize_ == SIZE_MAX) {
      auto size = VFS::length(file_);
      if (!size) {
        return 0;
      }
      cachedSize_ = size;
    }
    return cachedSize_;
  }

  void XEAI_IOStream::Flush() {
    if (file_.handle) {
      VFS::flush(file_);
    }
  }

}
