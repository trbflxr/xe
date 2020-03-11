//
// Created by trbflxr on 3/11/2020.
//

#ifndef XE_PARAMS_HPP
#define XE_PARAMS_HPP

#include <xe/serialization/node.hpp>

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

      //serialization
      friend const Node &operator>>(const Node &node, Params::Window &w) {
        node["width"].get(w.width);
        node["height"].get(w.height);
        node["title"].get(w.title);
        node["swapInterval"].get(w.swapInterval);
        node["srgb"].get(w.srgb);
        return node;
      }

      friend Node &operator<<(Node &node, const Params::Window &w) {
        node["width"].set(w.width);
        node["height"].set(w.height);
        node["title"].set(w.title);
        node["swapInterval"].set(w.swapInterval);
        node["srgb"].set(w.srgb);
        return node;
      }
    } window;

    struct GPU {
      uint32_t maxBuffers = 64;
      uint32_t maxTextures = 64;
      uint32_t maxPipelines = 32;
      uint32_t maxFramebuffers = 64;

      //serialization
      friend const Node &operator>>(const Node &node, Params::GPU &g) {
        node["maxBuffers"].get(g.maxBuffers);
        node["maxTextures"].get(g.maxTextures);
        node["maxPipelines"].get(g.maxPipelines);
        node["maxFramebuffers"].get(g.maxFramebuffers);
        return node;
      }

      friend Node &operator<<(Node &node, const Params::GPU &g) {
        node["maxBuffers"].set(g.maxBuffers);
        node["maxTextures"].set(g.maxTextures);
        node["maxPipelines"].set(g.maxPipelines);
        node["maxFramebuffers"].set(g.maxFramebuffers);
        return node;
      }
    } gpu;

    //serialization
    friend const Node &operator>>(const Node &node, Params &p) {
      node["gpu"].get(p.gpu);
      node["window"].get(p.window);
      return node;
    }

    friend Node &operator<<(Node &node, const Params &p) {
      node["gpu"].set(p.gpu);
      node["window"].set(p.window);
      return node;
    }
  };

}

#endif //XE_PARAMS_HPP
