//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_GPU_RESOURCES_HPP
#define XE_GPU_RESOURCES_HPP

#include <xe/common.hpp>

namespace xe {

  class RenderContext;

  namespace gpu {

    struct Resource {
      enum class ResourceType {
        Invalid,
        Buffer,
        Texture,
        Material,
        Framebuffer
      };

      RenderContext *ctx;
      ResourceType type;
      uint id;
    };

    struct Buffer : public Resource {
      Buffer(RenderContext *ctx = nullptr, uint id = 0) : Resource{ctx, ResourceType::Buffer, id} { }
      struct Info {
        BufferType type_;
        Usage usage_;
        uint size_;
        const char *name_;
      };
    };

  }

}

#endif //XE_GPU_RESOURCES_HPP
