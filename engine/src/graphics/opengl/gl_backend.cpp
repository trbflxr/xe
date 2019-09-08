//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "gl_backend.hpp"
#include "gl_shader_parser.hpp"
#include "external/glad/glad.h"
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

  static uint toGL(TextureMinFilter e) {
    switch (e) {
      case TextureMinFilter::Nearest: return GL_NEAREST;
      case TextureMinFilter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
      case TextureMinFilter::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
      case TextureMinFilter::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
      case TextureMinFilter::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
      case TextureMinFilter::Linear:
      default: return GL_LINEAR;
    }
  }

  static uint toGL(TextureMagFilter e) {
    switch (e) {
      case TextureMagFilter::Nearest: return GL_NEAREST;
      case TextureMagFilter::Linear:
      default: return GL_LINEAR;
    }
  }

  static uint toGL(TextureWrap e) {
    switch (e) {
      case TextureWrap::Repeat: return GL_REPEAT;
      case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
      case TextureWrap::Clamp:
      default: return GL_CLAMP_TO_EDGE;
    }
  }

  static uint toGL(BlendFactor e) {
    switch (e) {
      case BlendFactor::One: return GL_ONE;
      case BlendFactor::SrcColor: return GL_SRC_COLOR;
      case BlendFactor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
      case BlendFactor::SrcAlpha: return GL_SRC_ALPHA;
      case BlendFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
      case BlendFactor::DstColor: return GL_DST_COLOR;
      case BlendFactor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
      case BlendFactor::DstAlpha: return GL_DST_ALPHA;
      case BlendFactor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
      case BlendFactor::SrcAlphaSaturated: return GL_SRC_ALPHA_SATURATE;
      case BlendFactor::BlendColor: return GL_BLEND_COLOR;
      case BlendFactor::OneMinusBlendColor: return GL_ONE_MINUS_CONSTANT_COLOR;
      case BlendFactor::BlendAlpha: return GL_CONSTANT_ALPHA;
      case BlendFactor::OneMinusBlendAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
      case BlendFactor::Zero:
      default: return GL_ZERO;
    }
  }

  static uint toGL(BlendOp e) {
    switch (e) {
      case BlendOp::Substract: return GL_FUNC_SUBTRACT;
      case BlendOp::ReverseSubstract: return GL_FUNC_REVERSE_SUBTRACT;
      case BlendOp::Min: return GL_MIN;
      case BlendOp::Max: return GL_MAX;
      case BlendOp::Add:
      default: return GL_FUNC_ADD;
    }
  }

  static uint toGL(CompareFunc e) {
    switch (e) {
      case CompareFunc::Never: return GL_NEVER;
      case CompareFunc::Less: return GL_LESS;
      case CompareFunc::LessEqual: return GL_LEQUAL;
      case CompareFunc::Equal: return GL_EQUAL;
      case CompareFunc::NotEqual: return GL_NOTEQUAL;
      case CompareFunc::GreaterEqual: return GL_GEQUAL;
      case CompareFunc::Greater: return GL_GREATER;
      case CompareFunc::Always: return GL_ALWAYS;
      default: {
        XE_CORE_ERROR("[GL Error] Invalid compare func");
        return 0;
      }
    }
  }

  static uint toGL(IndexFormat e) {
    switch (e) {
      case IndexFormat::Uint16: return GL_UNSIGNED_SHORT;
      case IndexFormat::Uint32: return GL_UNSIGNED_INT;
      case IndexFormat::Uint8:
      default: return GL_UNSIGNED_BYTE;
    }
  }

  static uint toGL(Primitive e) {
    switch (e) {
      case Primitive::Lines: return GL_LINES;
      case Primitive::Points: return GL_POINTS;
      case Primitive::Triangles:
      default: return GL_TRIANGLES;
    }
  }

  static uint toGL(CubemapTarget e) {
    switch (e) {
      case CubemapTarget::PositiveX: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
      case CubemapTarget::NegativeX: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
      case CubemapTarget::PositiveY: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
      case CubemapTarget::NegativeY: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
      case CubemapTarget::PositiveZ: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
      case CubemapTarget::NegativeZ: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
      default: {
        XE_CORE_ERROR("[GL Error] Invalid cubemap target");
        return 0;
      }
    }
  }

  static uint vertexTypeToGL(uint format) {
    format = format & VertexFormat::TypeMask;
    switch (format) {
      case VertexFormat::Float: return GL_FLOAT;
      case VertexFormat::Int8: return GL_BYTE;
      case VertexFormat::Uint8: return GL_UNSIGNED_BYTE;
      case VertexFormat::Int16: return GL_SHORT;
      case VertexFormat::Uint16: return GL_UNSIGNED_SHORT;
      case VertexFormat::Int32: return GL_INT;
      case VertexFormat::Uint32: return GL_UNSIGNED_INT;
      default: {
        XE_CORE_ERROR("[GL Error] Invalid vertex format");
        return 0;
      }
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
#define GLCHECK(...) {__VA_ARGS__; checkGLError(__FILE__  ":" GLCHECK_STR(__LINE__) "\n\t-> " #__VA_ARGS__);}

  static void initTextureParams(uint target, const Texture::Info &info) {
    GLCHECK(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, toGL(info.minFilter)));
    GLCHECK(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, toGL(info.magFilter)));
    GLCHECK(glTexParameteri(target, GL_TEXTURE_WRAP_S, toGL(info.wrapping[0])));
    if (target > (uint) TextureType::T1D) {
      GLCHECK(glTexParameteri(target, GL_TEXTURE_WRAP_T, toGL(info.wrapping[1])));
    }
    if (target > (uint) TextureType::T2D) {
      GLCHECK(glTexParameteri(target, GL_TEXTURE_WRAP_R, toGL(info.wrapping[2])));
    }
  }

  static void initTexture(std::pair<TextureInstance *, Backend::Texture *> t) {
    uint id = t.second->texture;
    auto &backEnd = t.second;
    if (!id) {
      GLCHECK(glGenTextures(1, &id));
      backEnd->texture = id;
      t.first->id = id;

      switch (t.first->info.format) {
        case TexelsFormat::R8: {
          backEnd->format = GL_RED;
          backEnd->internalFormat = GL_R8;
          backEnd->type = GL_UNSIGNED_BYTE;
          break;
        }
        case TexelsFormat::Rg8: {
          backEnd->format = GL_RG;
          backEnd->internalFormat = GL_RG8;
          backEnd->type = GL_UNSIGNED_BYTE;
          break;
        }
        case TexelsFormat::Rgb8: {
          backEnd->format = GL_RGB;
          backEnd->internalFormat = GL_RGB8;
          backEnd->type = GL_UNSIGNED_BYTE;
          break;
        }
        case TexelsFormat::Rgba8: {
          backEnd->format = GL_RGBA;
          backEnd->internalFormat = GL_RGBA8;
          backEnd->type = GL_UNSIGNED_BYTE;
          break;
        }
        case TexelsFormat::R16f: {
          backEnd->format = GL_RED;
          backEnd->internalFormat = GL_R16F;
          backEnd->type = GL_FLOAT;
          break;
        }
        case TexelsFormat::Rg16f: {
          backEnd->format = GL_RG;
          backEnd->internalFormat = GL_RG16F;
          backEnd->type = GL_FLOAT;
          break;
        }
        case TexelsFormat::Rgb16f: {
          backEnd->format = GL_RGB;
          backEnd->internalFormat = GL_RGB16F;
          backEnd->type = GL_FLOAT;
          break;
        }
        case TexelsFormat::Rgba16f: {
          backEnd->format = GL_RGBA;
          backEnd->internalFormat = GL_RGBA16F;
          backEnd->type = GL_FLOAT;
          break;
        }
        case TexelsFormat::Depth16: {
          backEnd->format = GL_DEPTH_COMPONENT;
          backEnd->internalFormat = GL_DEPTH_COMPONENT16;
          backEnd->type = GL_UNSIGNED_SHORT;
          break;
        }
        case TexelsFormat::DepthStencil16: {
          backEnd->format = GL_DEPTH_STENCIL;
          backEnd->internalFormat = GL_DEPTH24_STENCIL8;
          backEnd->type = GL_UNSIGNED_SHORT;
          break;
        }
        case TexelsFormat::Depth24: {
          backEnd->format = GL_DEPTH_COMPONENT;
          backEnd->internalFormat = GL_DEPTH_COMPONENT24;
          backEnd->type = GL_UNSIGNED_INT;
          break;
        }
        case TexelsFormat::DepthStencil24: {
          backEnd->format = GL_DEPTH_STENCIL;
          backEnd->internalFormat = GL_DEPTH24_STENCIL8;
          backEnd->type = GL_UNSIGNED_INT;
          break;
        }
        default: break;
      }

      switch (t.first->info.type) {
        case TextureType::T1D: {
          t.second->target = GL_TEXTURE_1D;
          GLCHECK(glBindTexture(GL_TEXTURE_1D, id));
          GLCHECK(glTexImage1D(GL_TEXTURE_1D, 0, backEnd->internalFormat, t.first->info.width, 0,
                               backEnd->format, backEnd->type, nullptr));
          initTextureParams(GL_TEXTURE_1D, t.first->info);
          break;
        }
        case TextureType::T2D: {
          t.second->target = GL_TEXTURE_2D;
          GLCHECK(glBindTexture(GL_TEXTURE_2D, id));
          GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, backEnd->internalFormat, t.first->info.width,
                               t.first->info.height, 0, backEnd->format, backEnd->type, nullptr));
          initTextureParams(GL_TEXTURE_2D, t.first->info);
          break;
        }
        case TextureType::T3D: {
          t.second->target = GL_TEXTURE_3D;
          GLCHECK(glBindTexture(GL_TEXTURE_3D, id));
          GLCHECK(glTexImage3D(GL_TEXTURE_3D, 0, backEnd->internalFormat, t.first->info.width, t.first->info.height,
                               t.first->info.depth, 0, backEnd->format, backEnd->type, nullptr));
          initTextureParams(GL_TEXTURE_3D, t.first->info);
          break;
        }
        case TextureType::CubeMap: {
          t.second->target = GL_TEXTURE_CUBE_MAP;
          GLCHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
          for (uint16 i = 0; i < 6; ++i) {
            GLCHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, backEnd->internalFormat, t.first->info.width,
                                 t.first->info.height, 0, backEnd->format, backEnd->type, nullptr));
          }
          initTextureParams(GL_TEXTURE_CUBE_MAP, t.first->info);
          break;
        }
        default: break;
      }
    }
  }

  //todo: move to framebuffer init
  static void setupFramebufferTexture(std::pair<TextureInstance *, Backend::Texture *> t, uint mipLevel, uint16 i) {
    switch (t.second->target) {
      case GL_TEXTURE_2D: {
        GLCHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                                       GL_TEXTURE_2D, t.second->texture, mipLevel));
        break;
      }
      default: {
        XE_CORE_ERROR("[GL Error] Invalid texture type, expected texture 2D (color {}})", i);
        break;
      }
    }
  }

  //todo: move to framebuffer init
  static void setupFramebufferDepth(std::pair<TextureInstance *, Backend::Texture *> t, uint mipLevel) {
    if (t.second->target != GL_TEXTURE_2D) {
      XE_CORE_ERROR("[GL Error] Invalid texture type, expected texture 2D (depth/stencil)");
      return;
    }
    switch (t.first->info.format) {
      case TexelsFormat::Depth16:
      case TexelsFormat::Depth24: {
        GLCHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                       GL_TEXTURE_2D, t.second->texture, mipLevel));
        break;
      }
      case TexelsFormat::DepthStencil16:
      case TexelsFormat::DepthStencil24: {
        GLCHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                       GL_TEXTURE_2D, t.second->texture, mipLevel));
        break;
      }
      default: {
        XE_CORE_ERROR("[GL Error] Invalid texels-format for a depth/stencil texture");
      }
    }
  }

  static int32 compileShader(uint type, const char *src) {
    if (src[0] == '\0') {
      return -1;
    }

    int32 shader = 0;
    GLCHECK(shader = glCreateShader(type));
    if (!shader) {
      XE_CORE_ERROR("[GL Error] Could not create shader program");
      return 0;
    }
    int32 compiled = 0;
    GLCHECK(glShaderSource(shader, 1, &src, nullptr));
    GLCHECK(glCompileShader(shader));
    GLCHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));

    if (!compiled) {
      static const size_t maxLength = 2048;
      char buffer[maxLength];
      GLCHECK(glGetShaderInfoLog(shader, maxLength, nullptr, buffer));
      XE_CORE_ERROR("[GL Error] Could compile shader ({}):{}\n// CODE: {}", type, buffer, src);
      GLCHECK(glDeleteShader(shader));
      return 0;
    }
    return shader;
  }

  static bool parseOrError(const string &source, Backend::Uniform *uniforms, size_t &outUsedUniforms, size_t &outSize) {
    if (!GLShaderParser::parse(source, uniforms, outUsedUniforms, outSize)) {
      XE_CORE_ERROR("[GL Error] Max shader uniforms reached ({}/{})", cMaxShaderUniforms, outUsedUniforms);
    }
    return true;
  }

  static void setUniform(int32 loc, uint count, VertexFormat::Enum type, const void *data) {
    switch (type) {
      case VertexFormat::Float: {
        GLCHECK(glUniform1f(loc, *(float *) data));
        break;
      }
      case VertexFormat::Float2: {
        const float *v = (const float *) data;
        GLCHECK(glUniform2f(loc, v[0], v[1]));
        break;
      }
      case VertexFormat::Float3: {
        const float *v = (const float *) data;
        GLCHECK(glUniform3f(loc, v[0], v[1], v[2]));
        break;
      }
      case VertexFormat::Float4: {
        const float *v = (const float *) data;
        GLCHECK(glUniform4f(loc, v[0], v[1], v[2], v[3]));
        break;
      }
      case VertexFormat::Int32: {
        glUniform1i(loc, *(int32 *) data);
        break;
      }
      case VertexFormat::Mat4: {
        GLCHECK(glUniformMatrix4fv(loc, count, GL_TRUE, (const float *) data));
        break;
      }
      default: break;
    }
  }

  void Backend::initBackend(Backend **b, const Params::GPU &params) {
    *b = new Backend();

    (*b)->buffers_.alloc(params.maxBuffers);
    (*b)->textures_.alloc(params.maxTextures);
    (*b)->pipelines_.alloc(params.maxPipelines);
    (*b)->framebuffers_.alloc(params.maxFramebuffers);
  }

  void Backend::destroyBackend(Backend **b) {
    delete *b;
    *b = nullptr;
  }

  void Backend::clear(const DisplayList::ClearData &d) {
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

  void Backend::setupView(DisplayList::ViewData &d) {
    if (d.framebuffer.id != 0) {
      auto fb = RenderContext::getResource(d.framebuffer.id, &d.framebuffer.ctx->framebuffers_,
                                           &d.framebuffer.ctx->backend_->framebuffers_);
      uint fbId = fb.second->framebuffer;
      if (!fbId) {
        GLCHECK(glGenFramebuffers(1, &fbId));
        fb.second->framebuffer = fbId;
      }

      GLCHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbId));

      if (d.cubemapTarget != CubemapTarget::Invalid) {
        for (uint16 i = 0; i < fb.first->info.colorAttachmentsSize; ++i) {
          auto tex = RenderContext::getResource(fb.first->colorAttachments[i].id,
                                                &fb.first->colorAttachments[i].ctx->textures_,
                                                &fb.first->colorAttachments[i].ctx->backend_->textures_);
          initTexture(tex);
          if (tex.second->target == GL_TEXTURE_CUBE_MAP) {
            GLCHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, toGL(d.cubemapTarget),
                                           tex.second->texture, d.mipLevel));
          }
        }
      } else {
        for (uint16 i = 0; i < fb.first->info.colorAttachmentsSize; ++i) {
          auto tex = RenderContext::getResource(fb.first->colorAttachments[i].id,
                                                &fb.first->colorAttachments[i].ctx->textures_,
                                                &fb.first->colorAttachments[i].ctx->backend_->textures_);
          initTexture(tex);
          setupFramebufferTexture(tex, d.mipLevel, i);
        }

        if (RenderContext::checkValidResource(fb.first->depthAttachment.id, &d.framebuffer.ctx->textures_)) {
          auto tex = RenderContext::getResource(fb.first->depthAttachment.id,
                                                &fb.first->depthAttachment.ctx->textures_,
                                                &fb.first->depthAttachment.ctx->backend_->textures_);
          initTexture(tex);
          setupFramebufferDepth(tex, d.mipLevel);
        }
      }

      //todo: move
      memory<uint> buffers;
      buffers.alloc(fb.first->info.colorAttachmentsSize);
      uint count = 0;
      for (uint i = 0; i < fb.first->info.colorAttachmentsSize; ++i) {
        if (d.colorAttachment[i]) {
          buffers[count] = GL_COLOR_ATTACHMENT0 + i;
          ++count;
        }
      }

      if (count > 0) {
        GLCHECK(glDrawBuffers(count, buffers.data.get()));
      } else {
        GLCHECK(glDrawBuffer(GL_NONE));
      }

    } else {
      GLCHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    if (d.viewport.width != 0 && d.viewport.height != 0) {
      GLCHECK(glViewport(d.viewport.x, d.viewport.y, d.viewport.width, d.viewport.height));
    }
    GLCHECK(glDisable(GL_SCISSOR_TEST));
  }

  void Backend::fillBuffer(DisplayList::FillBufferData &d) {
    auto b = RenderContext::getResource(d.buffer.id, &d.buffer.ctx->buffers_, &d.buffer.ctx->backend_->buffers_);
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

  void Backend::fillTexture(DisplayList::FillTextureData &d) {
    auto t = RenderContext::getResource(d.texture.id, &d.texture.ctx->textures_, &d.texture.ctx->backend_->textures_);

    d.width = d.width ? d.width : t.first->info.width;
    d.height = d.height ? d.height : t.first->info.height;
    d.depth = d.depth ? d.depth : t.first->info.depth;
    if (d.width != t.first->info.width || d.height != t.first->info.height || d.depth != t.first->info.depth) {
      t.first->info.width = d.width;
      t.first->info.height = d.height;
      t.first->info.depth = d.depth;
    }

    initTexture(t);
    auto &backEnd = *t.second;

    GLCHECK(glBindTexture(backEnd.target, backEnd.texture));
    GLCHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    switch (t.first->info.type) {
      case TextureType::T1D: {
        if (d.data0 != nullptr) {
          GLCHECK(glTexSubImage1D(GL_TEXTURE_1D, 0, d.offsetX, d.width, backEnd.format, backEnd.type, d.data0));
        }
        break;
      }
      case TextureType::T2D: {
        if (d.data0 != nullptr) {
          GLCHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, d.offsetX, d.offsetY, d.width, d.height, backEnd.format,
                                  backEnd.type, d.data0));
        }
        break;
      }
      case TextureType::T3D: {
        if (d.data0 != nullptr) {
          GLCHECK(glTexSubImage3D(GL_TEXTURE_3D, 0, d.offsetX, d.offsetY, d.offsetZ, d.width, d.height, d.depth,
                                  backEnd.format, backEnd.type, d.data0));
        }
        break;
      }
      case TextureType::CubeMap: {
        void *data[6] = {(void *) d.data0, (void *) d.data1, (void *) d.data2,
                         (void *) d.data3, (void *) d.data4, (void *) d.data5};
        for (uint16 i = 0; i < 6; ++i) {
          if (data[i] != nullptr) {
            GLCHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, d.offsetX, d.offsetY, d.width, d.height,
                                    backEnd.format, backEnd.type, data[i]));
          }
        }
        break;
      }
      default: break;
    }
    if (d.buildMipmap) {
      GLCHECK(glGenerateMipmap(backEnd.target));
    }
  }

  void Backend::setupPipeline(DisplayList::SetupPipelineData &d) {
    bool lastPipelineChanged = d.pipeline.id != d.pipeline.ctx->lastPipeline_.pipeline.id;
    d.pipeline.ctx->lastPipeline_ = d;
    auto mat = RenderContext::getResource(d.pipeline.ctx->lastPipeline_.pipeline.id, &d.pipeline.ctx->pipelines_,
                                          &d.pipeline.ctx->backend_->pipelines_);
    if (lastPipelineChanged) {
      //shader creation
      if (mat.second->program == 0) {
        for (int32 &i : mat.second->textureUniformsLoc) {
          i = -1;
        }

        const int32 shaderV = compileShader(GL_VERTEX_SHADER, mat.first->vertShader.c_str());
        const int32 shaderF = compileShader(GL_FRAGMENT_SHADER, mat.first->fragShader.c_str());
        const int32 shaderTC = compileShader(GL_TESS_CONTROL_SHADER, mat.first->tessControlShader.c_str());
        const int32 shaderTE = compileShader(GL_TESS_EVALUATION_SHADER, mat.first->tessEvalShader.c_str());
        const int32 shaderG = compileShader(GL_GEOMETRY_SHADER, mat.first->geomShader.c_str());
        if (!shaderV || !shaderF || !shaderTC || !shaderTE || !shaderG) {
          return;
        }

        uint programId = 0;
        GLCHECK(programId = glCreateProgram());
        if (!programId) {
          XE_CORE_ERROR("[GL Error] Could not create shader program");
          return;
        }

        uint bufferSize = 0;
        if (shaderV != -1) {
          GLCHECK(glAttachShader(programId, shaderV));
          if (!parseOrError(mat.first->vertShader, mat.second->uniforms,
                            mat.second->usedUniforms, bufferSize)) {
            return;
          }
        }
        if (shaderF != -1) {
          GLCHECK(glAttachShader(programId, shaderF));
          if (!parseOrError(mat.first->fragShader, mat.second->uniforms,
                            mat.second->usedUniforms, bufferSize)) {
            return;
          }
        }
        if (shaderTC != -1) {
          GLCHECK(glAttachShader(programId, shaderTC));
          if (!parseOrError(mat.first->tessControlShader, mat.second->uniforms,
                            mat.second->usedUniforms, bufferSize)) {
            return;
          }
        }
        if (shaderTE != -1) {
          GLCHECK(glAttachShader(programId, shaderTE));
          if (!parseOrError(mat.first->tessEvalShader, mat.second->uniforms,
                            mat.second->usedUniforms, bufferSize)) {
            return;
          }
        }
        if (shaderG != -1) {
          GLCHECK(glAttachShader(programId, shaderG));
          if (!parseOrError(mat.first->geomShader, mat.second->uniforms,
                            mat.second->usedUniforms, bufferSize)) {
            return;
          }
        }

        mat.second->uniformData.alloc(bufferSize);

        for (auto i = 0; i < cMaxVertexAttribs; ++i) {
          const auto &attrib = mat.first->info.attribs[i];
          if (attrib.format) {
            GLCHECK(glBindAttribLocation(programId, i, attrib.name));
          } else {
            break;
          }
        }

        GLCHECK(glLinkProgram(programId));
        int32 linkStatus = GL_FALSE;
        GLCHECK(glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus));
        if (linkStatus != GL_TRUE) {
          char log[2048];
          GLCHECK(glGetProgramInfoLog(programId, 2048, nullptr, log));
          GLCHECK(glDeleteProgram(programId));
          XE_CORE_ERROR("[GL Error] Could not link program:\n\t{}", log);
          return;
        }
        mat.second->program = programId;

        if (shaderV != -1) GLCHECK(glDeleteShader(shaderV));
        if (shaderF != -1) GLCHECK(glDeleteShader(shaderF));
        if (shaderTC != -1) GLCHECK(glDeleteShader(shaderTC));
        if (shaderTE != -1) GLCHECK(glDeleteShader(shaderTE));
        if (shaderG != -1) GLCHECK(glDeleteShader(shaderG));

        //get texture locations
        for (size_t i = 0; i < cMaxTextureUnits; ++i) {
          char name[32];
          switch (mat.first->info.textures[i]) {
            case TextureType::T1D: snprintf(name, 32, "u_tex1d%d", i);
              break;
            case TextureType::T2D: snprintf(name, 32, "u_tex2d%d", i);
              break;
            case TextureType::T3D: snprintf(name, 32, "u_tex3d%d", i);
              break;
            case TextureType::CubeMap: snprintf(name, 32, "u_texCube%d", i);
              break;
            default: break;
          }

          if ((uint) mat.first->info.textures[i]) {
            int32 loc;
            GLCHECK(loc = glGetUniformLocation(programId, name));
            if (loc != -1) {
              mat.second->textureUniformsLoc[i] = loc;
            } else {
              XE_CORE_ERROR("[GL Error] Unable to find texture uniform '{}' (pipeline {})", name, d.pipeline.id);
            }
          }
        }

        auto &&p = mat.second;
        for (size_t i = 0; i < p->usedUniforms; ++i) {
          GLCHECK(p->uniforms[i].loc = glGetUniformLocation(p->program, p->uniforms[i].name.c_str()));
        }
      }

      GLCHECK(glUseProgram(mat.second->program));

      if (mat.first->info.blend.enabled) {
        GLCHECK(glEnable(GL_BLEND));
        vec4 c = mat.first->info.blend.color;
        GLCHECK(glBlendColor(c[0], c[1], c[2], c[3]));
        GLCHECK(glBlendEquationSeparate(toGL(mat.first->info.blend.opRgb),
                                        toGL(mat.first->info.blend.opAlpha)));
        GLCHECK(glBlendFuncSeparate(toGL(mat.first->info.blend.srcRgb),
                                    toGL(mat.first->info.blend.dstRgb),
                                    toGL(mat.first->info.blend.srcAlpha),
                                    toGL(mat.first->info.blend.dstAlpha)));
      } else {
        GLCHECK(glDisable(GL_BLEND));
      }

      switch (mat.first->info.cull) {
        case Cull::Front: GLCHECK(glEnable(GL_CULL_FACE));
          GLCHECK(glCullFace(GL_FRONT));
          break;
        case Cull::Back: GLCHECK(glEnable(GL_CULL_FACE));
          GLCHECK(glCullFace(GL_BACK));
          break;
        case Cull::Disabled: GLCHECK(glDisable(GL_CULL_FACE));
          break;
      }

      const byte colorWrite = (mat.first->info.rgbaWrite) ? GL_TRUE : GL_FALSE;
      const byte depthWrite = (mat.first->info.depthWrite) ? GL_TRUE : GL_FALSE;
      GLCHECK(glColorMask(colorWrite, colorWrite, colorWrite, colorWrite));
      GLCHECK(glDepthMask(depthWrite));

      if (mat.first->info.depthFunc != CompareFunc::Disabled) {
        GLCHECK(glDepthFunc(toGL(mat.first->info.depthFunc)));
        GLCHECK(glEnable(GL_DEPTH_TEST));
      } else {
        GLCHECK(glDisable(GL_DEPTH_TEST));
      }

      for (size_t i = 0; i < cMaxVertexAttribs; ++i) {
        const auto &attrib = mat.first->info.attribs[i];
        if (attrib.format) {
          GLCHECK(glEnableVertexAttribArray(i));
          switch (attrib.vertexStep) {
            case VertexStep::PerVertex: GLCHECK(glVertexAttribDivisor(i, 0));
              break;
            case VertexStep::PerInstance: GLCHECK(glVertexAttribDivisor(i, 1));
              break;
          }
        } else {
          GLCHECK(glDisableVertexAttribArray(i));
        }
      }

      for (size_t i = 0; i < cMaxUniformBuffers; ++i) {
        if (d.uniformBuffer[i].id) {
          auto ubo = RenderContext::getResource(d.uniformBuffer[i].id, &d.pipeline.ctx->buffers_,
                                                &d.pipeline.ctx->backend_->buffers_);
          uint uniformIndex = 0;
          GLCHECK(uniformIndex = glGetUniformBlockIndex(mat.second->program, ubo.first->info.name_));
          GLCHECK(glUniformBlockBinding(mat.second->program, uniformIndex, ubo.second->buffer));
        }
      }
    }

    //todo: push model matrix
//    GLCHECK(glUniformMatrix4fv(mat.second->modelLoc, 1, GL_TRUE, (const float *) &d.modelMatrix));
//    GLCHECK(glUniformMatrix4fv(mat.second->viewLoc, 1, GL_TRUE, (const float *) &viewMatrix_));
//    GLCHECK(glUniformMatrix4fv(mat.second->projLoc, 1, GL_TRUE, (const float *) &projMatrix_));


    for (auto &&u : d.uniform) {
      if (!u.name) {
        break;
      }
      for (size_t j = 0; j < mat.second->usedUniforms; ++j) {
        auto &&mu = mat.second->uniforms[j];
        if (mu.name == u.name) {
          memcpy(mat.second->uniformData.data.get() + mu.offset, u.data, mu.size);
          setUniform(mu.loc, mu.count, mu.type, mat.second->uniformData.data.get() + mu.offset);
          break;
        }
      }
    }


    if (d.scissor[2] > 0.0f && d.scissor[3] > 0.0f) {
      GLCHECK(glScissor((int32) d.scissor[0], (int32) d.scissor[1], (int32) d.scissor[2], (int32) d.scissor[3]));
      GLCHECK(glEnable(GL_SCISSOR_TEST));
    } else {
      GLCHECK(glDisable(GL_SCISSOR_TEST));
    }
  }

  void Backend::render(DisplayList::RenderData &d) {
    auto buf = RenderContext::getResource(d.indexBuffer.id, &d.indexBuffer.ctx->buffers_,
                                          &d.indexBuffer.ctx->backend_->buffers_);

    auto mat = RenderContext::getResource(d.indexBuffer.ctx->lastPipeline_.pipeline.id, &d.indexBuffer.ctx->pipelines_,
                                          &d.indexBuffer.ctx->backend_->pipelines_);

    if (mat.first->info.renderMode == RenderMode::Disabled) {
      return;
    }

    if (!buf.second->buffer) {
      XE_CORE_ERROR("[GL Error] Invalid index buffer");
    }

    auto &pipeline = d.indexBuffer.ctx->lastPipeline_;

    size_t texUnit = 0;
    for (auto i = 0; i < cMaxTextureUnits; ++i) {
      if (mat.second->textureUniformsLoc[i] >= 0) {
        auto tex = RenderContext::getResource(pipeline.texture[i].id, &d.indexBuffer.ctx->textures_,
                                              &d.indexBuffer.ctx->backend_->textures_);
        if (!tex.first) {
          XE_CORE_ERROR("[GL Error] Missing texture");
          return;
        }

        GLCHECK(glActiveTexture(GL_TEXTURE0 + texUnit));
        switch (tex.first->info.type) {
          case TextureType::T1D: GLCHECK(glBindTexture(GL_TEXTURE_1D, tex.second->texture));
            break;
          case TextureType::T2D: GLCHECK(glBindTexture(GL_TEXTURE_2D, tex.second->texture));
            break;
          case TextureType::T3D: GLCHECK(glBindTexture(GL_TEXTURE_3D, tex.second->texture));
            break;
          case TextureType::CubeMap: GLCHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, tex.second->texture));
            break;
          default: XE_CORE_ERROR("[GL Error] Invalid texture type");
            return;
        }

        GLCHECK(glUniform1i(mat.second->textureUniformsLoc[i], texUnit));
        ++texUnit;
      }
    }

    for (auto i = 0; i < cMaxVertexAttribs; ++i) {
      const uint attribFormat = mat.first->info.attribs[i].format;
      if (attribFormat) {
        const size_t bufferIndex = mat.first->info.attribs[i].bufferIndex;
        const size_t bufferId = pipeline.buffer[bufferIndex].id;
        if (!bufferId) {
          XE_CORE_ERROR("[GL Error] Expected valid buffer (see pipeline declaration)");
          return;
        }
        auto buffer = RenderContext::getResource(bufferId, &d.indexBuffer.ctx->buffers_,
                                                 &d.indexBuffer.ctx->backend_->buffers_);
        if (buffer.second->buffer == 0) {
          XE_CORE_ERROR("[GL Error] Invalid GL buffer (vertex data)");
        }

        //todo: refactor
        GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer.second->buffer));
        int32 a_size = (attribFormat & VertexFormat::NumComponentsMask) >> VertexFormat::NumComponentsShift;
        uint a_type = vertexTypeToGL(attribFormat);
        byte a_normalized = (attribFormat & VertexFormat::Normalized) ? GL_TRUE : GL_FALSE;
        int32 a_stride = mat.first->info.attribs[i].stride;
        int32 a_offset = mat.first->info.attribs[i].offset;
        GLCHECK(glVertexAttribPointer(i, a_size, a_type, a_normalized, a_stride, (void *) a_offset));
        glEnableVertexAttribArray(i);
      } else {
        break;
      }
    }

    switch (mat.first->info.renderMode) {
      case RenderMode::Solid: {
        GLCHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(toGL(mat.first->info.primitive),
                                        d.count, toGL(d.type), (void *) d.offset, d.instances));
        break;
      }
      case RenderMode::Wireframe: {
        GLCHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(toGL(mat.first->info.primitive),
                                        d.count, toGL(d.type), (void *) d.offset, d.instances));
        break;
      }
      default: break;
    }

  }

}
