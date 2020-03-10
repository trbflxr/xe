//
// Created by FLXR on 6/18/2019.
//

#include "xepch.hpp"
#include <xe/core/vfs.hpp>
#include <xe/core/filesystem.hpp>

namespace xe {

  VFS &VFS::ref() {
    static std::unique_ptr<VFS> vfs;
    if (!vfs) {
      vfs.reset(new VFS());
    }
    return *vfs;
  }

  void VFS::mount(std::string_view virtualPath, std::string_view physicalPath) {
    ref().mountPoints_[virtualPath.data()].push_back(physicalPath.data());
  }

  void VFS::unmount(std::string_view virtualPath, std::string_view physicalPath) {
    std::vector<std::string> &temp = ref().mountPoints_[virtualPath.data()];
    temp.erase(std::remove_if(temp.begin(), temp.end(), [&](std::string_view pp) {
                 return pp == physicalPath;
               }),
               temp.end());
  }

  void VFS::unmount(std::string_view virtualPath) {
    ref().mountPoints_[virtualPath.data()].clear();
  }

  bool VFS::resolvePhysicalPath(std::string_view path, std::string &outPhysicalPath) {
    if (path[0] != '/') {
      outPhysicalPath = path;
      return FileSystem::exists(path);
    }

    std::vector<std::string> dirs = string::split(path, '/');
    const std::string &virtualDir = dirs.front();

    if (ref().mountPoints_.find(virtualDir) == ref().mountPoints_.end() ||
        ref().mountPoints_[virtualDir].empty()) {
      return false;
    }

    std::string remainder = std::string(path.substr(virtualDir.size() + 2, path.size() - virtualDir.size() - 1));
    for (const std::string &physicalPath : ref().mountPoints_[virtualDir]) {
      std::string p = (physicalPath != "/" ? physicalPath : "") + remainder;
      if (FileSystem::exists(p)) {
        outPhysicalPath = p;
        return true;
      }
    }
    return false;
  }

  byte *VFS::readFile(std::string_view path, int64 *outSize) {
    std::string physicalPath;

    if (resolvePhysicalPath(path, physicalPath)) {
      return FileSystem::read(physicalPath, outSize);
    } else {
      XE_CORE_ERROR("[VFS] Unable find file '{}'", path);
      return nullptr;
    }
  }

  bool VFS::readTextFile(std::string_view path, std::string &outString) {
    std::string physicalPath;

    if (resolvePhysicalPath(path, physicalPath)) {
      return FileSystem::readText(physicalPath, outString);
    } else {
      XE_CORE_ERROR("[VFS] Unable find file '{}'", path);
      outString = "";
      return false;
    }
  }

  bool VFS::writeFile(std::string_view path, byte *buffer, size_t size) {
    std::string physicalPath;
    return resolvePhysicalPath(path, physicalPath) ? FileSystem::write(physicalPath, buffer, size) : false;
  }

}
