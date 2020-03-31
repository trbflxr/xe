//
// Created by trbflxr on 3/3/2020.
//

#ifndef XE_VFS_HPP
#define XE_VFS_HPP

#include <vector>
#include <string>
#include <optional>
#include <xe/common.hpp>

namespace xe {

  class XE_API VFS {
    friend class Engine;
  public:
    struct File {
      void *handle = nullptr;
      std::string path;
    };

    enum class OpenMode {
      Read,
      Write,
      Append
    };

    enum class Seek {
      Set,
      Cur,
      End
    };

  public:
    ~VFS();

    void mount(std::string_view dir);
    void unmount(std::string_view dir);
    void unmountAll();

    static bool mounted(std::string_view dir);

    static bool exists(std::string_view file);

    static bool isDirectory(std::string_view path);
    static bool isRegularFile(std::string_view path);

    static bool read(File &file, void *buffer, size_t size);
    static std::optional<std::string> readText(std::string_view file);
    static std::vector<uint8_t> readBytes(std::string_view file);

    static std::optional<std::string> getWriteDir();
    static bool setWriteDir(std::string_view dir);

    static bool write(File &file, const void *data, size_t size);
    static bool write(std::string_view file, const void *data, size_t size, bool append = false);

    static bool mkdir(std::string_view dir);

    static std::vector<std::string> filesInDir(std::string_view dir, bool recursive = true);

    static bool close(File &file);
    static File open(std::string_view file, OpenMode mode);

    static bool seek(File &file, Seek origin, int64_t offset);
    static size_t tell(File &file);
    static size_t length(File &file);
    static bool flush(File &file);

  private:
    VFS();

  private:
    std::vector<std::string> mountPoints_;
  };

}

#endif //XE_VFS_HPP
