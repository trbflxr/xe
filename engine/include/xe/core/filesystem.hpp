//
// Created by FLXR on 6/18/2019.
//

#ifndef XE_FILESYSTEM_HPP
#define XE_FILESYSTEM_HPP

#include <xe/string.hpp>
#include <xe/vector.hpp>
#include <xe/utils/noncopyable.hpp>

namespace xe {

  class XE_API FileSystem : NonCopyable {
  public:
    static bool exists(const string &file);
    static int64 size(const string &file);

    static byte *read(const string &file, int64 *outSize = nullptr);
    static bool read(const string &file, void *buff, int64 size = -1);
    static bool readText(const string &file, string &outString);

    static bool write(const string &file, void *buff, size_t size);

    static string getWorkingDirectory();

    static vector<string> getLogicalDrives();

    static void updateVolumes() { ref().updateVolumes_ = true; }

  private:
    FileSystem();
    static FileSystem &ref();

  private:
    bool updateVolumes_;
  };

}

#endif //XE_FILESYSTEM_HPP
