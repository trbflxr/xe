//
// Created by FLXR on 6/18/2019.
//

#include "xepch.hpp"
#include <xe/core/filesystem.hpp>

#pragma warning(disable:4996) //warning C4996: '_wfopen'

namespace xe {

  FileSystem::FileSystem() :
      updateVolumes_(true) { }

  FileSystem &FileSystem::ref() {
    static std::unique_ptr<FileSystem> fs;
    if (!fs) {
      fs.reset(new FileSystem());
    }
    return *fs;
  }

#ifdef XE_GENERIC_FS
  static FILE *openFileForReading(const string &path) {
    return _wfopen(path.toWide().c_str(), L"rb");
  }

  static int64 getFileSizeInternal(FILE *file) {
    int64 size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
  }

  static bool readFileInternal(FILE *file, void *buffer, size_t size) {
    fread(buffer, size, 1, file);
    return true;
  }

  bool FileSystem::exists(const string &file) {
    FILE *f = openFileForReading(file);
    if (f) {
      fclose(f);
      return true;
    }

    return false;
  }

  int64 FileSystem::size(const string &file) {
    FILE *f = openFileForReading(file);
    if (!f) return -1;

    const int64 size = getFileSizeInternal(f);

    fclose(f);
    return size;
  }

  byte *FileSystem::read(const string &file, int64 *outSize) {
    FILE *handle = openFileForReading(file);
    const size_t size = static_cast<size_t>(getFileSizeInternal(handle));
    byte *buffer = new byte[size];

    const bool success = readFileInternal(handle, buffer, size);
    fclose(handle);

    if (!success) {
      delete[] buffer;
      return nullptr;
    }

    if (outSize) {
      *outSize = size;
    }

    return buffer;
  }

  bool FileSystem::read(const string &file, void *buff, int64 size) {
    FILE *handle = openFileForReading(file);
    if (!handle) return false;

    if (size < 0) {
      size = getFileSizeInternal(handle);
    }
    const size_t s = static_cast<size_t>(size);
    const bool success = readFileInternal(handle, buff, s);
    fclose(handle);

    return success;
  }

  bool FileSystem::readText(const string &file, string &outString) {
    FILE *handle = openFileForReading(file);
    const size_t size = static_cast<size_t>(getFileSizeInternal(handle));
    std::string buffer(size, '\0');

    bool success = readFileInternal(handle, &buffer[0], size);
    fclose(handle);

    if (success) {
      buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
      outString = buffer;
      return true;
    } else {
      return false;
    }
  }

  bool FileSystem::write(const string &file, void *buff, size_t size) {
    FILE *f = _wfopen(file.toWide().c_str(), L"wb");
    if (!f) return false;

    fwrite(buff, size, 1, f);
    fclose(f);

    return true;
  }
#endif //XE_GENERIC_FS

}
