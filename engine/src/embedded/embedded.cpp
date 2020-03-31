//
// Created by FLXR on 9/11/2019.
//

#include "xepch.hpp"
#include "embedded.hpp"

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(xe);

namespace xe {

  const gpu::Texture::Info &Embedded::defaultTextureInfo() {
    static constexpr const uint8_t size = 4;
    static gpu::Texture::Info info;

    if (info.format == TexelsFormat::None) {
      info.width = size;
      info.height = size;
      info.minFilter = TextureMinFilter::Nearest;
      info.magFilter = TextureMagFilter::Nearest;
      info.wrapS = TextureWrap::Repeat;
      info.wrapT = TextureWrap::Repeat;
      info.wrapR = TextureWrap::Repeat;
      info.format = TexelsFormat::Rgb8;
      info.type = TextureType::T2D;
    }

    return info;
  }

  const void *Embedded::defaultTextureData() {
    static constexpr const uint8_t data[48] = {
        255, 0, 255, 0, 0, 0, 255, 0, 255, 0, 0, 0,
        0, 0, 0, 255, 0, 255, 0, 0, 0, 255, 0, 255,
        255, 0, 255, 0, 0, 0, 255, 0, 255, 0, 0, 0,
        0, 0, 0, 255, 0, 255, 0, 0, 0, 255, 0, 255,
    };
    return data;
  }

  std::vector<uint8_t> Embedded::defaultFount() {
    auto fs = cmrc::xe::get_filesystem();
    auto data = fs.open("xe/resources/Hack-Regular.ttf");

    return {data.begin(), data.end()};
  }

  std::string Embedded::shaderSource(const std::string &name) {
#ifdef XE_PLATFORM_GL
    const std::string path = "xe/src/graphics/opengl/shaders/" + name;
#else
  #error only gl currently supported
#endif

    auto fs = cmrc::xe::get_filesystem();
    auto data = fs.open(path);

    return {data.begin(), data.end()};
  }

}
