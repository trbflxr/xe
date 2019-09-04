//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_RENDER_CONTEXT_HPP
#define XE_RENDER_CONTEXT_HPP

#include <xe/common.hpp>
#include <xe/scoped_array.hpp>
#include <xe/graphics/gpu_instances.hpp>
#include <xe/utils/logger.hpp>

namespace xe {

  namespace gpu {
    struct BackEnd;
    void fillBuffer(DrawList::FillBufferData &d);
    void render(DrawList::RenderData &d);
  }

  class XE_API RenderContext {
    friend class GPU;
    friend void gpu::fillBuffer(DrawList::FillBufferData &d);
    friend void gpu::render(DrawList::RenderData &d);
  public:
    RenderContext();
    ~RenderContext();

    void init(const Params::GPU &params);

    static uint index(uint id);
    static std::pair<uint, uint> indexAndVersion(uint id);

    template<class T>
    static bool checkValidResource(uint id, const scoped_array<T> *pool) {
      auto pv = indexAndVersion(id);
      uint loc = pv.first;
      uint version = pv.second;
      const T *result = &(*pool)[loc];
      uint realVersion = (result->state.load() & 0xFFF);

      return realVersion == version;
    }

  private:
    template<class T, class B>
    static std::pair<T *, B *> getResource(uint id, scoped_array<T> *instanceArray, scoped_array<B> *backendArray) {
      if (checkValidResource(id, instanceArray)) {
        uint i = index(id);
        return {&(*instanceArray)[i], &(*backendArray)[i]};
      }
      XE_CORE_ERROR("[RenderContext] Invalid resource (id: {})", id);
      return {nullptr, nullptr};
    }

  private:
    gpu::BackEnd *backEnd_;

    scoped_array<gpu::BufferInstance> buffers_;

  };

}

#endif //XE_RENDER_CONTEXT_HPP
