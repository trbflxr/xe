//
// Created by FLXR on 6/18/2019.
//

#ifndef XE_FILESYSTEM_HPP
#define XE_FILESYSTEM_HPP

#include <vector>
#include <xe/string.hpp>
#include <xe/utils/noncopyable.hpp>

namespace xe {

  class XE_API FileSystem : NonCopyable {
  public:
    static bool exists(std::string_view file);
    static int64 size(std::string_view file);

    static byte *read(std::string_view file, int64 *outSize = nullptr);
    static bool read(std::string_view file, void *buff, int64 size = -1);
    static bool readText(std::string_view file, std::string &outString);

    static bool write(std::string_view file, void *buff, size_t size);

    static std::string getWorkingDirectory();

    static std::vector<std::string> getLogicalDrives();

    static void updateVolumes() { ref().updateVolumes_ = true; }

  private:
    FileSystem();
    static FileSystem &ref();

  private:
    bool updateVolumes_;
  };

}

#endif //XE_FILESYSTEM_HPP
