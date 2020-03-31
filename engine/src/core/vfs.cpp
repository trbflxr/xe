//
// Created by trbflxr on 3/3/2020.
//

#include "xepch.hpp"
#include <xe/core/vfs.hpp>

#include <physfs.h>
#include <xe/utils/logger.hpp>
#include <xe/core/engine.hpp>

namespace xe {

  VFS::VFS() {
    if (!PHYSFS_init(Engine::ref().getArgs()[0].c_str())) {
      XE_CORE_ERROR("[VFS] Failed to initialize filesystem. Code: {}",
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return;
    }
    mount(".");
    setWriteDir(".");
  }

  VFS::~VFS() {
    PHYSFS_deinit();
  }

  void VFS::mount(std::string_view dir) {
    if (std::find(mountPoints_.begin(), mountPoints_.end(), dir) != mountPoints_.end()) {
      XE_CORE_WARN("[VFS] directory '{}' already mounted", dir);
      return;
    }

    if (!PHYSFS_addToSearchPath(dir.data(), true)) {
      XE_CORE_ERROR("[VFS] Failed to mount directory '{}'. Code: {}", dir,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return;
    }
    XE_CORE_INFO("[VFS] Directory '{}' mounted", dir);

    mountPoints_.emplace_back(dir);
  }

  void VFS::unmount(std::string_view dir) {
    auto it = std::find(mountPoints_.begin(), mountPoints_.end(), dir);

    if (it == mountPoints_.end()) {
      XE_CORE_WARN("[VFS] directory '{}' not mounted", dir);
      return;
    }

    if (!PHYSFS_removeFromSearchPath(dir.data())) {
      XE_CORE_ERROR("[VFS] Failed to unmount directory '{}'. Code: {}", dir,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return;
    }
    XE_CORE_INFO("[VFS] Directory '{}' unmounted", dir);

    mountPoints_.erase(it);
  }

  void VFS::unmountAll() {
    for (const auto &dir : mountPoints_) {
      if (!PHYSFS_removeFromSearchPath(dir.c_str())) {
        XE_CORE_ERROR("[VFS] Failed to unmount directory '{}'. Code: {}", dir,
                      PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      }
    }
    XE_CORE_INFO("[VFS] All directories unmounted");

    mountPoints_.clear();
  }

  bool VFS::mounted(std::string_view dir) {
    if (!PHYSFS_isInit()) {
      XE_CORE_ERROR("[VFS] VFS is not initialized");
      return false;
    }

    for (auto sp = PHYSFS_getSearchPath(); *sp != nullptr; ++sp) {
      if (strcmp(dir.data(), *sp) == 0) {
        return true;
      }
    }
    return false;
  }

  bool VFS::exists(std::string_view file) {
    if (!PHYSFS_isInit()) {
      XE_CORE_ERROR("[VFS] VFS is not initialized");
      return false;
    }
    return PHYSFS_exists(file.data());
  }

  bool VFS::isDirectory(std::string_view path) {
    PHYSFS_Stat stat{ };

    if (!PHYSFS_stat(path.data(), &stat)) {
      XE_CORE_ERROR("[VFS] Failed to open path '{}'. Code: {}", path,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return false;
    }
    return stat.filetype == PHYSFS_FILETYPE_DIRECTORY;
  }

  bool VFS::isRegularFile(std::string_view path) {
    PHYSFS_Stat stat{ };

    if (!PHYSFS_stat(path.data(), &stat)) {
      XE_CORE_ERROR("[VFS] Failed to open path '{}'. Code: {}", path,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return false;
    }
    return stat.filetype == PHYSFS_FILETYPE_REGULAR;
  }

  bool VFS::read(VFS::File &file, void *buffer, size_t size) {
#if XE_DEBUG_TRACING
    const std::string name = std::string("VFS Read file ") + file.path;
#else
    static const std::string name("VFS Read file");
#endif

    XE_TRACE_BEGIN("XE", name.c_str());

    auto rb = PHYSFS_readBytes(reinterpret_cast<PHYSFS_File *>(file.handle), buffer, size);
    if (rb == -1 || !rb) {
      return false;
    }

    if (static_cast<size_t>(rb) == size) {
      XE_CORE_INFO("[VFS] Read file '{}', ({} / {} bytes)", file.path, rb, size);
    } else {
      XE_CORE_WARN("[VFS] Read file '{}', ({} / {} bytes)", file.path, rb, size);
    }

    XE_TRACE_END("XE", name.c_str());
    return true;
  }

  std::optional<std::string> VFS::readText(std::string_view file) {
    std::vector<uint8_t> bytes = readBytes(file);
    if (bytes.empty()) {
      return std::nullopt;
    }
    return std::string(bytes.begin(), bytes.end());
  }

  std::vector<uint8_t> VFS::readBytes(std::string_view file) {
    File fsFile = open(file, OpenMode::Read);
    if (!fsFile.handle) {
      return { };
    }
    auto size = length(fsFile);
    if (!size) {
      return { };
    }

    std::vector<uint8_t> data(size);
    if (!read(fsFile, data.data(), size)) {
      close(fsFile);
      return { };
    }

    close(fsFile);
    return data;
  }

  std::optional<std::string> VFS::getWriteDir() {
    auto dir = PHYSFS_getWriteDir();
    if (!dir) {
      XE_CORE_WARN("[VFS] No write dir currently set");
      return std::nullopt;
    }
    return dir;
  }

  bool VFS::setWriteDir(std::string_view dir) {
    if (!PHYSFS_setWriteDir(dir.data())) {
      XE_CORE_ERROR("[VFS] Failed to set write directory '{}'. Code: {}", dir,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return false;
    }

    XE_CORE_INFO("[VFS] Write dir set to '{}'", dir);
    return true;
  }

  bool VFS::write(VFS::File &file, const void *data, size_t size) {
#if XE_DEBUG_TRACING
    const std::string name = std::string("VFS Write file ") + file.path;
#else
    static const std::string name("VFS Write file");
#endif

    XE_TRACE_BEGIN("XE", name.c_str());

    auto written = PHYSFS_writeBytes(reinterpret_cast<PHYSFS_File *>(file.handle), data, static_cast<PHYSFS_uint64>(size));
    if (written == -1 || static_cast<size_t>(written) != size) {
      XE_CORE_ERROR("[VFS] Failed to write / append file '{}', bytes written {}. Code: {}", file.path,
                    written, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

      close(file);
      XE_TRACE_END("XE", name.c_str());
      return false;
    }

    XE_TRACE_END("XE", name.c_str());
    return true;
  }

  bool VFS::write(std::string_view file, const void *data, size_t size, bool append) {
    File fsFile = open(file, append ? OpenMode::Append : OpenMode::Write);
    if (!fsFile.handle) {
      return false;
    }

    bool result = write(fsFile, data, size);
    if (result) {
      XE_CORE_INFO("[VFS] File '{}' successfully write", file);
    }

    close(fsFile);
    return result;
  }

  bool VFS::mkdir(std::string_view dir) {
    auto result = PHYSFS_mkdir(dir.data());

    if (!result) {
      XE_CORE_ERROR("[VFS] Failed to create directory '{}'. Code: {}", dir,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return false;
    }

    XE_CORE_INFO("[VFS] Directory '{}' successfully created", dir);

    return true;
  }

  std::vector<std::string> VFS::filesInDir(std::string_view dir, bool recursive) {
    auto fsFiles = PHYSFS_enumerateFiles(dir.data());

    std::vector<std::string> files;

    for (auto file = fsFiles; *file; ++file) {
      std::string recursiveDir(dir);
      recursiveDir.append("/").append(*file);
      bool isDir = isDirectory(recursiveDir);
      if (recursive && isDir) {
        auto filesInFound = filesInDir(recursiveDir, recursive);
        for (auto &&f : filesInFound) {
          std::string path(*file);
          path.append("/");
          f.insert(0, path);
        }
        files.insert(files.end(), filesInFound.begin(), filesInFound.end());
      } else {
        if (!isDir) {
          files.emplace_back(*file);
        }
      }
    }

    PHYSFS_freeList(fsFiles);
    return files;
  }

  bool VFS::close(VFS::File &file) {
    if (!PHYSFS_close(reinterpret_cast<PHYSFS_File *>(file.handle))) {
      XE_CORE_ERROR("[VFS] Failed to close file '{}'. Code: {}", file.path,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return false;
    }
    file.handle = nullptr;
    return true;
  }

  VFS::File VFS::open(std::string_view file, OpenMode mode) {
    PHYSFS_File *handle = nullptr;
    const char *log = "error";

    switch (mode) {
      case OpenMode::Read: {
        handle = PHYSFS_openRead(file.data());
        log = "read";
        break;
      }
      case OpenMode::Write: {
        handle = PHYSFS_openWrite(file.data());
        log = "write";
        break;
      }
      case OpenMode::Append: {
        handle = PHYSFS_openAppend(file.data());
        log = "append";
        break;
      }
      default: break;
    }

    if (!handle) {
      XE_CORE_ERROR("[VFS] Failed to open ({}) file '{}'. Code: {}", log, file,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }

    return {handle, file.data()};
  }

  bool VFS::seek(VFS::File &file, Seek origin, int64_t offset) {
    XE_ASSERT(file.handle, "Unable to seek nullptr file");

    size_t len = length(file);
    XE_ASSERT(static_cast<size_t>(offset) <= len, "Unable to seek file, offset: {}, file length: {}", offset, len);

    auto *pfsFile = reinterpret_cast<PHYSFS_File *>(file.handle);

    int32_t res = 0;
    switch (origin) {
      case Seek::Cur: {
        res = PHYSFS_seek(pfsFile, PHYSFS_tell(pfsFile) + offset);
        break;
      }
      case Seek::End: {
        res = PHYSFS_seek(pfsFile, len + offset);
        break;
      }
      default:
      case Seek::Set: {
        res = PHYSFS_seek(pfsFile, offset);
        break;
      }
    }

    if (!res) {
      XE_CORE_ERROR("[VFS] Failed to seek file '{}'. Code: {}", file.path,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return false;
    }
    return true;

  }

  size_t VFS::tell(VFS::File &file) {
    XE_ASSERT(file.handle, "Unable to tell nullptr file");

    const int64_t r = PHYSFS_tell(reinterpret_cast<PHYSFS_File *>(file.handle));
    if (r == -1) {
      XE_CORE_ERROR("[VFS] Failed to tell file '{}'. Code: {}", file.path,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return 0;
    }
    return static_cast<size_t>(r);
  }

  size_t VFS::length(VFS::File &file) {
    XE_ASSERT(file.handle, "Unable to get size from nullptr file");

    auto size = PHYSFS_fileLength(reinterpret_cast<PHYSFS_File *>(file.handle));
    if (size == -1) {
      XE_CORE_ERROR("[VFS] Failed to determinate size for '{}'", file.path);
      return 0;
    }
    return static_cast<size_t>(size);
  }

  bool VFS::flush(VFS::File &file) {
    XE_ASSERT(file.handle, "Unable to flush nullptr file");

    if (!PHYSFS_flush(reinterpret_cast<PHYSFS_File *>(file.handle))) {
      XE_CORE_ERROR("[VFS] Failed to flush file '{}'. Code: {}", file.path,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return false;
    }
    return true;
  }

}
