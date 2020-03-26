//
// Created by FLXR on 9/11/2019.
//

#ifndef XE_EMBEDDED_HPP
#define XE_EMBEDDED_HPP

#include <xe/common.hpp>
#include <xe/utils/noncopyable.hpp>
#include <xe/graphics/gpu_resources.hpp>

namespace xe {

  class Embedded : NonCopyable {
  public:
    static const gpu::Texture::Info &defaultTextureInfo();
    static const void *defaultTextureData();

    static std::vector<uint8_t> defaultFount();
  };

}

#endif //XE_EMBEDDED_HPP
