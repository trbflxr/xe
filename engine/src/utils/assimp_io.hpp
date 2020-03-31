//
// Created by trbflxr on 3/30/2020.
//

#ifndef XE_ASSIMP_IO_HPP
#define XE_ASSIMP_IO_HPP

#include <xe/core/vfs.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/IOStream.hpp>

namespace Assimp {

  class XEAI_IOSystem : public IOSystem {
  public:
    bool Exists(const char *file) const override;

    char getOsSeparator() const override;

    IOStream *Open(const char *file, const char *mode) override;
    void Close(IOStream *file) override;

    bool ComparePaths(const char *one, const char *second) const override;
  };


  class XEAI_IOStream : public IOStream {
    friend class XEAI_IOSystem;
  protected:
    XEAI_IOStream() noexcept;
    explicit XEAI_IOStream(xe::VFS::File &&handle);

  public:
    ~XEAI_IOStream() override;

    size_t Read(void *buffer, size_t size, size_t count) override;

    size_t Write(const void *buffer, size_t size, size_t count) override;
    aiReturn Seek(size_t offset, aiOrigin origin) override;

    size_t Tell() const override;
    size_t FileSize() const override;

    void Flush() override;

  private:
    mutable xe::VFS::File file_;
    mutable size_t cachedSize_;
  };

}

#endif //XE_ASSIMP_IO_HPP
