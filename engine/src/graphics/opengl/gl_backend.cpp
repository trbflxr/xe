//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "gl_backend.hpp"
#include <glad/glad.h>

namespace xe::gpu {

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

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_DEPTH_TEST);
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

}
