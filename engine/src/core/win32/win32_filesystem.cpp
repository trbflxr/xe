//
// Created by FLXR on 6/18/2019.
//


#include "xepch.hpp"
#include <xe/core/filesystem.hpp>

namespace xe {

#ifndef XE_GENERIC_FS
  void CALLBACK fileIOCompletionRoutine(DWORD errorCode, DWORD numberOfBytesTransfered, LPOVERLAPPED overlapped) { }

  static HANDLE openFileForReading(const string &path) {
    return CreateFileW(path.toWide().c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, nullptr);
  }

  static int64 getFileSizeInternal(HANDLE file) {
    LARGE_INTEGER size;
    GetFileSizeEx(file, &size);
    return size.QuadPart;
  }

  static bool readFileInternal(HANDLE file, void *buffer, int64 size) {
    OVERLAPPED ol = {0};
    return (bool) ReadFileEx(file, buffer, static_cast<DWORD>(size), &ol, fileIOCompletionRoutine);
  }

  bool FileSystem::exists(const string &file) {
    const DWORD dwAttrib = GetFileAttributesW(file.toWide().c_str());
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
  }

  int64 FileSystem::size(const string &file) {
    HANDLE handle = openFileForReading(file);
    if (handle == INVALID_HANDLE_VALUE) return -1;

    int64 result = getFileSizeInternal(handle);
    CloseHandle(handle);

    return result;
  }

  byte *FileSystem::read(const string &file, int64 *outSize) {
    HANDLE handle = openFileForReading(file);
    const int64 s = getFileSizeInternal(handle);
    const size_t size = static_cast<size_t>(s);
    if (s <= 0) {
      CloseHandle(handle);
      return nullptr;
    }
    byte *buffer = new byte[size];

    bool success = readFileInternal(handle, buffer, size);
    CloseHandle(handle);

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
    HANDLE handle = openFileForReading(file);
    if (handle == INVALID_HANDLE_VALUE) return false;

    if (size < 0) {
      size = getFileSizeInternal(handle);
    }

    bool success = readFileInternal(handle, buff, size);
    CloseHandle(handle);

    return success;
  }

  bool FileSystem::readText(const string &file, string &outString) {
    HANDLE handle = openFileForReading(file);
    const int64 s = getFileSizeInternal(handle);
    const size_t size = static_cast<size_t>(s);
    if (s <= 0) {
      CloseHandle(handle);
      return false;
    }
    std::string buffer(size, '\0');

    bool success = readFileInternal(handle, &buffer[0], size);
    CloseHandle(handle);

    if (success) {
      buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
      outString = buffer;
      return true;
    } else {
      return false;
    }
  }

  bool FileSystem::write(const string &file, void *buff, size_t size) {
    DWORD mode = exists(file) ? OPEN_EXISTING : CREATE_NEW;
    HANDLE handle = CreateFileW(file.toWide().c_str(), GENERIC_WRITE, 0,
                                nullptr, mode, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (handle == INVALID_HANDLE_VALUE) return false;

    DWORD written;
    const int32 result = ::WriteFile(handle, buff, static_cast<DWORD>(size), &written, nullptr);

    CloseHandle(handle);

    return static_cast<bool>(result);
  }
#endif //XE_GENERIC_FS

#ifdef XE_PLATFORM_WINDOWS
  string FileSystem::getWorkingDirectory() {
    static constexpr uint maxSize = 512;

    wchar_t buffer[maxSize];
    GetCurrentDirectoryW(maxSize, buffer);

    return buffer;
  }

  vector<string> FileSystem::getLogicalDrives() {
    static vector<string> drives;

    if (ref().updateVolumes_) {
      ref().updateVolumes_ = false;

      drives.clear();

      DWORD d = GetLogicalDrives();
      for (uint i = 0; i < 26; ++i) { //A-Z
        if ((d & (1 << i))) {
          const char r[] = {static_cast<char>('A' + i), ':', '/', '\0'};
          drives.emplace_back(r);
        }
      }
    }
    return drives;
  }
#endif

}
