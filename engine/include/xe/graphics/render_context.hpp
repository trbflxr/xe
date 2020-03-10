//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_RENDER_CONTEXT_HPP
#define XE_RENDER_CONTEXT_HPP

#include <xe/common.hpp>
#include <xe/memory.hpp>
#include <xe/graphics/display_list.hpp>
#include <xe/graphics/gpu_instances.hpp>
#include <xe/utils/logger.hpp>

namespace xe {

  namespace gpu {
    class Backend;
    struct Framebuffer;
  }

  class XE_API RenderContext {
    friend class GPU;
    friend class gpu::Backend;
    friend struct gpu::Framebuffer;
  public:
    RenderContext();
    ~RenderContext();

    void init(const Params::GPU &params);

    static uint32_t index(uint32_t id);
    static std::pair<uint32_t, uint32_t> indexAndVersion(uint32_t id);

    static size_t computeSize(VertexFormat::Enum type);

    template<class T>
    static bool checkValidResource(uint32_t id, const memory<T> *pool) {
      auto pv = indexAndVersion(id);
      uint32_t loc = pv.first;
      uint32_t version = pv.second;
      const T *result = &(*pool)[loc];
      uint32_t realVersion = (result->state.load() & 0xFFF);

      return realVersion == version;
    }

  private:
    template<class T, class B>
    static std::pair<T *, B *> getResource(uint32_t id, memory<T> *instanceArray, memory<B> *backendArray) {
      if (checkValidResource(id, instanceArray)) {
        uint32_t i = index(id);
        return {&(*instanceArray)[i], &(*backendArray)[i]};
      }
      XE_CORE_ERROR("[RenderContext] Invalid resource (id: {})", id);
      return {nullptr, nullptr};
    }

  private:
    gpu::Backend *backend_;

    memory<gpu::BufferInstance> buffers_;
    memory<gpu::TextureInstance> textures_;
    memory<gpu::PipelineInstance> pipelines_;
    memory<gpu::FramebufferInstance> framebuffers_;

    DisplayList::SetupPipelineData lastPipeline_;
  };

}

#endif //XE_RENDER_CONTEXT_HPP
