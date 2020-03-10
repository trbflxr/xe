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

  void VFS::mount(const string &virtualPath, const string &physicalPath) {
    ref().mountPoints_[virtualPath].push_back(physicalPath);
  }

  void VFS::unmount(const string &virtualPath, const string &physicalPath) {
    std::vector<string> &temp = ref().mountPoints_[virtualPath];
    temp.erase(std::remove_if(temp.begin(), temp.end(), [&](const string &pp) {
                 return pp == physicalPath;
               }),
               temp.end());
  }

  void VFS::unmount(const string &virtualPath) {
    ref().mountPoints_[virtualPath].clear();
  }

  bool VFS::resolvePhysicalPath(const string &path, string &outPhysicalPath) {
    if (path[0] != '/') {
      outPhysicalPath = path;
      return FileSystem::exists(path);
    }

    std::vector<string> dirs = path.split('/');
    const string &virtualDir = dirs.front();

    if (ref().mountPoints_.find(virtualDir) == ref().mountPoints_.end() ||
        ref().mountPoints_[virtualDir].empty()) {
      return false;
    }

    string remainder = path.substr(virtualDir.size() + 2, path.size() - virtualDir.size() - 1);
    for (const string &physicalPath : ref().mountPoints_[virtualDir]) {
      string p = (physicalPath != "/" ? physicalPath : "") + remainder;
      if (FileSystem::exists(p)) {
        outPhysicalPath = p;
        return true;
      }
    }
    return false;
  }

  byte *VFS::readFile(const string &path, int64 *outSize) {
    string physicalPath;

    if (resolvePhysicalPath(path, physicalPath)) {
      return FileSystem::read(physicalPath, outSize);
    } else {
      XE_CORE_ERROR("[VFS] Unable find file '{}'", path);
      return nullptr;
    }
  }

  bool VFS::readTextFile(const string &path, string &outString) {
    string physicalPath;

    if (resolvePhysicalPath(path, physicalPath)) {
      return FileSystem::readText(physicalPath, outString);
    } else {
      XE_CORE_ERROR("[VFS] Unable find file '{}'", path);
      outString = "";
      return false;
    }
  }

  bool VFS::writeFile(const string &path, byte *buffer, size_t size) {
    string physicalPath;
    return resolvePhysicalPath(path, physicalPath) ? FileSystem::write(physicalPath, buffer, size) : false;
  }

}
