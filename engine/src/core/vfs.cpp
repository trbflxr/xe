//
// Created by trbflxr on 3/3/2020.
//

#include "xepch.hpp"
#include <xe/core/vfs.hpp>

#include <physfs.h>
#include <xe/utils/logger.hpp>
#include <xe/core/engine.hpp>

namespace xe::detail {

  enum class OpenMode {
    Read,
    Write,
    Append
  };

  bool closeFileHandle(PHYSFS_File *handle, std::string_view file) {
    if (!PHYSFS_close(handle)) {
      XE_CORE_ERROR("[VFS] Failed to close file '{}'. Code: {}", file,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return false;
    }
    return true;
  }

  PHYSFS_File *openFileHandle(std::string_view file, OpenMode mode) {
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

    return handle;
  }

}

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
    mountPoints_.erase(it);
  }

  void VFS::unmountAll() {
    for (const auto &dir : mountPoints_) {
      if (!PHYSFS_removeFromSearchPath(dir.c_str())) {
        XE_CORE_ERROR("[VFS] Failed to unmount directory '{}'. Code: {}", dir,
                      PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      }
    }
    mountPoints_.clear();
  }

  bool VFS::mounted(std::string_view dir) {
    if (!PHYSFS_isInit()) {
      XE_CORE_WARN("[VFS] VFS is not initialized");
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
      XE_CORE_WARN("[VFS] VFS is not initialized");
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

  std::optional<std::string> VFS::readText(std::string_view file) {
    std::vector<uint8_t> bytes = readBytes(file);
    if (bytes.empty()) {
      return std::nullopt;
    }
    return std::string(bytes.begin(), bytes.end());
  }

  std::vector<uint8_t> VFS::readBytes(std::string_view file) {
#if XE_DEBUG_TRACING
    const std::string name = std::string("VFS Read file ") + file.data();
#else
    static const std::string name("VFS Read file");
#endif

    XE_TRACE_BEGIN("XE", name.c_str());
    auto fsFile = detail::openFileHandle(file, detail::OpenMode::Read);
    if (!fsFile) {
      XE_TRACE_END("XE", name.c_str());
      return { };
    }

    auto size = PHYSFS_fileLength(fsFile);
    std::vector<uint8_t> data(size);
    PHYSFS_readBytes(fsFile, data.data(), static_cast<PHYSFS_uint64>(size));

    detail::closeFileHandle(fsFile, file);

    XE_TRACE_END("XE", (std::string("VFS Read file ") + file.data()).c_str());
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

  bool VFS::write(std::string_view file, void *data, size_t size, bool append) {
#if XE_DEBUG_TRACING
    const std::string name = std::string("VFS Write file ") + file.data();
#else
    static const std::string name("VFS Write file");
#endif

    XE_TRACE_BEGIN("XE", name.c_str());
    auto fsFile = detail::openFileHandle(file, append ? detail::OpenMode::Append : detail::OpenMode::Write);
    if (!fsFile) {
      XE_TRACE_END("XE", name.c_str());
      return false;
    }

    auto written = PHYSFS_writeBytes(fsFile, data, static_cast<PHYSFS_uint64>(size));
    if (written == -1 || static_cast<size_t>(written) != size) {
      XE_CORE_ERROR("[VFS] Failed to {} file '{}', bytes written {}. Code: {}", append ? "append" : "write", file,
                    written, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

      detail::closeFileHandle(fsFile, file);
      XE_TRACE_END("XE", name.c_str());
      return false;
    }

    detail::closeFileHandle(fsFile, file);
    XE_TRACE_END("XE", name.c_str());
    return true;
  }

  bool VFS::mkdir(std::string_view dir) {
    auto result = PHYSFS_mkdir(dir.data());

    if (!result) {
      XE_CORE_ERROR("[VFS] Failed to create directory '{}'. Code: {}", dir,
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      return false;
    }
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

}
