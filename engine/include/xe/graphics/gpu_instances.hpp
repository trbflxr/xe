//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_GPU_INSTANCES_HPP
#define XE_GPU_INSTANCES_HPP

#include <atomic>
#include <xe/graphics/gpu_resources.hpp>

namespace xe::gpu {

  struct InstanceBase {
    uint version = 0;
    std::atomic<uint> state = 0;

    bool acquire() {
      uint v = (version + 1);
      if (!v) {
        v = 1;
      }
      uint e = 0;
      if (state.compare_exchange_weak(e, v)) {
        version = v;
        return true;
      }
      return false;
    }

    void release() {
      state = 0;
    }
  };

  struct BufferInstance : public InstanceBase {
    Buffer::Info info;
  };

  struct TextureInstance : public InstanceBase {
    Texture::Info info;
    size_t bpp;
    uint id;
  };

  struct MaterialInstance : public InstanceBase {
    Material::Info info;
    string vertShader;
    string fragShader;
    string attributes[cMaxVertexAttribs];
  };

  struct FramebufferInstance : public InstanceBase {
    Framebuffer::Info info;
    Texture colorAttachments[cMaxFramebufferColorAttachments];
    Texture depthAttachment;
  };

}

#endif //XE_GPU_INSTANCES_HPP
