//
// Created by FLXR on 9/11/2019.
//

#include "xepch.hpp"
#include "embedded.hpp"

namespace xe {

  const gpu::Texture::Info &Embedded::defaultTextureInfo() {
    static constexpr const byte size = 4;
    static gpu::Texture::Info info;

    if (info.format == TexelsFormat::None) {
      info.width = size;
      info.height = size;
      info.minFilter = TextureMinFilter::Nearest;
      info.magFilter = TextureMagFilter::Nearest;
      info.wrapping[0] = TextureWrap::Repeat;
      info.wrapping[1] = TextureWrap::Repeat;
      info.wrapping[2] = TextureWrap::Repeat;
      info.format = TexelsFormat::Rgb8;
      info.type = TextureType::T2D;
    }

    return info;
  }

  const void *Embedded::defaultTextureData() {
    static constexpr const byte data[48] = {
        255, 0, 255, 0, 0, 0, 255, 0, 255, 0, 0, 0,
        0, 0, 0, 255, 0, 255, 0, 0, 0, 255, 0, 255,
        255, 0, 255, 0, 0, 0, 255, 0, 255, 0, 0, 0,
        0, 0, 0, 255, 0, 255, 0, 0, 0, 255, 0, 255,
    };
    return data;
  }

}
