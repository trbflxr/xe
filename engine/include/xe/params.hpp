//
// Created by trbflxr on 3/11/2020.
//

#ifndef XE_PARAMS_HPP
#define XE_PARAMS_HPP

#include <string>
#include <xe/common.hpp>

namespace xe {

  struct Params {
    struct Window {
      uint32_t width = 0;
      uint32_t height = 0;
      std::string title = "xe";
      bool swapInterval = false;
      bool srgb = true;

      uint32_t iconWidth = 0;
      uint32_t iconHeight = 0;
      uint8_t *iconPixels = nullptr;
    } window;

    struct GPU {
      uint32_t maxBuffers = 64;
      uint32_t maxTextures = 64;
      uint32_t maxPipelines = 32;
      uint32_t maxFramebuffers = 64;
    } gpu;
  };

}

#endif //XE_PARAMS_HPP
