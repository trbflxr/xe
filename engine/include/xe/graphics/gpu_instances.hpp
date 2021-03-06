//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_GPU_INSTANCES_HPP
#define XE_GPU_INSTANCES_HPP

#include <atomic>
#include <xe/graphics/gpu_resources.hpp>

namespace xe::gpu {

  struct InstanceBase {
    uint32_t version = 0;
    std::atomic<uint32_t> state = 0;

    bool acquire() {
      uint32_t v = (version + 1);
      if (!v) {
        v = 1;
      }
      uint32_t e = 0;
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
    uint32_t id;
    uint64_t handle;
  };

  struct PipelineInstance : public InstanceBase {
    Pipeline::Info info;
    std::string vertShader;
    std::string tessControlShader;
    std::string tessEvalShader;
    std::string geomShader;
    std::string fragShader;
    std::string attributes[cMaxVertexAttribs];
  };

  struct FramebufferInstance : public InstanceBase {
    Framebuffer::Info info;
    std::shared_ptr<Texture> colorAttachments[cMaxFramebufferColorAttachments];
    std::shared_ptr<Texture> depthAttachment;
  };

}

#endif //XE_GPU_INSTANCES_HPP
