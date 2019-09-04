//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "gl_backend.hpp"
#include <glad/glad.h>
#include <xe/graphics/render_context.hpp>

namespace xe::gpu {

  static uint toGL(Usage e) {
    switch (e) {
      case Usage::Dynamic: return GL_DYNAMIC_DRAW;
      case Usage::Stream: return GL_STREAM_DRAW;
      case Usage::Static:
      default: return GL_STATIC_DRAW;
    }
  }

  static uint toGL(BufferType e) {
    switch (e) {
      case BufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
      case BufferType::Uniform: return GL_UNIFORM_BUFFER;
      case BufferType::Vertex:
      default: return GL_ARRAY_BUFFER;
    }
  }

  static void checkGLError(const char *operation) {
    int32 error = glGetError();
    if (error) {
      XE_CORE_CRITICAL("[GL Error] {} ({})", error, operation);
      return;
    }
  }

#define GLCHECK_STR_STR(A) #A
#define GLCHECK_STR(A) GLCHECK_STR_STR(A)
#define GLCHECK(...) {__VA_ARGS__; checkGLError(__FILE__  ":" GLCHECK_STR(__LINE__) "->" #__VA_ARGS__);}

  void initBackEnd(BackEnd **b, const Params::GPU &params) {
    *b = new BackEnd();

    (*b)->buffers.alloc(params.maxBuffers);
    (*b)->textures.alloc(params.maxTextures);
    (*b)->materials.alloc(params.maxMaterials);
    (*b)->framebuffers.alloc(params.maxFramebuffers);

//    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//    glEnable(GL_DEPTH_TEST);
  }

  void destroyBackEnd(BackEnd **b) {
    delete *b;
    *b = nullptr;
  }

  void clear(const DrawList::ClearData &d) {
    uint mask = 0;
    if (d.clearColor) {
      GLCHECK(glClearColor(d.color.r, d.color.g, d.color.b, d.color.a));
      mask |= GL_COLOR_BUFFER_BIT;
      GLCHECK(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
    }
    if (d.clearDepth) {
      GLCHECK(glClearDepth(d.depth));
      mask |= GL_DEPTH_BUFFER_BIT;
      GLCHECK(glDepthMask(GL_TRUE));
    }
    if (d.clearStencil) {
      GLCHECK(glClearStencil(d.stencil));
      mask |= GL_STENCIL_BUFFER_BIT;
    }
    GLCHECK(glClear(mask));
  }

  void fillBuffer(DrawList::FillBufferData &d) {
    auto b = RenderContext::getResource(d.buffer.id, &d.buffer.ctx->buffers_, &d.buffer.ctx->backEnd_->buffers);
    uint id = b.second->buffer;
    const uint target = toGL(b.first->info.type_);

    if (!id) {
      GLCHECK(glGenBuffers(1, &id));
      GLCHECK(glBindBuffer(target, id));
      GLCHECK(glBufferData(target, d.size, nullptr, toGL(b.first->info.usage_)));
      b.second->buffer = id;
    }

    GLCHECK(glBindBuffer(target, id));

    if (b.first->info.size_ != d.size) {
      GLCHECK(glBufferData(target, d.size, nullptr, toGL(b.first->info.usage_)));
      b.first->info.size_ = d.size;
    }

    GLCHECK(glBufferSubData(target, d.offset, d.size, d.data));

    if (target == GL_UNIFORM_BUFFER) {
      GLCHECK(glBindBufferBase(target, id, id));
    }
  }

  void render(DrawList::RenderData &d) {
    auto buf = RenderContext::getResource(d.indexBuffer.id, &d.indexBuffer.ctx->buffers_,
                                          &d.indexBuffer.ctx->backEnd_->buffers);

    if (!buf.second->buffer) {
      XE_CORE_ERROR("[GL Error] Invalid Index buffer");
    }

  }

}
