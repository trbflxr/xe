//
// Created by FLXR on 6/18/2019.
//

#ifndef XE_VFS_HPP
#define XE_VFS_HPP

#include <xe/memory.hpp>
#include <xe/string.hpp>
#include <xe/vector.hpp>
#include <xe/unordered_map.hpp>
#include <xe/utils/noncopyable.hpp>
#include <xe/core/filesystem.hpp>

namespace xe {

  class XE_API VFS : NonCopyable {
  public:
    static void mount(const string &virtualPath, const string &physicalPath);
    static void unmount(const string &virtualPath, const string &physicalPath);
    static void unmount(const string &virtualPath);

    static bool resolvePhysicalPath(const string &path, string &outPhysicalPath);

    static byte *readFile(const string &path, int64 *outSize = nullptr);
    static bool readTextFile(const string &path, string &outString);

    static bool writeFile(const string &path, byte *buffer, size_t size);

  private:
    VFS() = default;

    static VFS &ref();

  private:
    unordered_map<string, vector<string>> mountPoints_;
  };

}

#endif //XE_VFS_HPP
