//
// Created by FLXR on 6/18/2019.
//

#ifndef XE_VFS_HPP
#define XE_VFS_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <xe/string.hpp>
#include <xe/utils/noncopyable.hpp>
#include <xe/core/filesystem.hpp>

namespace xe {

  class XE_API VFS : NonCopyable {
  public:
    static void mount(std::string_view virtualPath, std::string_view physicalPath);
    static void unmount(std::string_view virtualPath, std::string_view physicalPath);
    static void unmount(std::string_view virtualPath);

    static bool resolvePhysicalPath(std::string_view path, std::string &outPhysicalPath);

    static byte *readFile(std::string_view path, int64 *outSize = nullptr);
    static bool readTextFile(std::string_view path, std::string &outString);

    static bool writeFile(std::string_view path, byte *buffer, size_t size);

  private:
    VFS() = default;

    static VFS &ref();

  private:
    std::unordered_map<std::string, std::vector<std::string>> mountPoints_;
  };

}

#endif //XE_VFS_HPP
