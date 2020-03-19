//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include "gl_backend.hpp"

#include "gl_shader_parser.hpp"
#include "embedded/embedded.hpp"
#include "external/glad/glad.h"
#include <xe/core/gpu.hpp>
#include <xe/core/engine.hpp>
#include <xe/graphics/render_context.hpp>

namespace xe::gpu {

  static uint32_t toGL(Usage e) {
    switch (e) {
      case Usage::Dynamic: return GL_DYNAMIC_DRAW;
      case Usage::Stream: return GL_STREAM_DRAW;
      case Usage::Static:
      default: return GL_STATIC_DRAW;
    }
  }

  static uint32_t toGL(BufferType e) {
    switch (e) {
      case BufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
      case BufferType::Uniform: return GL_UNIFORM_BUFFER;
      case BufferType::Vertex:
      default: return GL_ARRAY_BUFFER;
    }
  }

  static uint32_t toGL(TextureMinFilter e) {
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

  static uint32_t toGL(TextureMagFilter e) {
    switch (e) {
      case TextureMagFilter::Nearest: return GL_NEAREST;
      case TextureMagFilter::Linear:
      default: return GL_LINEAR;
    }
  }

  static uint32_t toGL(TextureWrap e) {
    switch (e) {
      case TextureWrap::Repeat: return GL_REPEAT;
      case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
      case TextureWrap::Clamp:
      default: return GL_CLAMP_TO_EDGE;
    }
  }

  static uint32_t toGL(BlendFactor e) {
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

  static uint32_t toGL(BlendOp e) {
    switch (e) {
      case BlendOp::Substract: return GL_FUNC_SUBTRACT;
      case BlendOp::ReverseSubstract: return GL_FUNC_REVERSE_SUBTRACT;
      case BlendOp::Min: return GL_MIN;
      case BlendOp::Max: return GL_MAX;
      case BlendOp::Add:
      default: return GL_FUNC_ADD;
    }
  }

  static uint32_t toGL(CompareFunc e) {
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

  static uint32_t toGL(IndexFormat e) {
    switch (e) {
      case IndexFormat::Uint16: return GL_UNSIGNED_SHORT;
      case IndexFormat::Uint32: return GL_UNSIGNED_INT;
      case IndexFormat::Uint8:
      default: return GL_UNSIGNED_BYTE;
    }
  }

  static uint32_t toGL(Primitive e) {
    switch (e) {
      case Primitive::Lines: return GL_LINES;
      case Primitive::Points: return GL_POINTS;
      case Primitive::Triangles:
      default: return GL_TRIANGLES;
    }
  }

  static uint32_t toGL(CubemapTarget e) {
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

  static uint32_t vertexTypeToGL(uint32_t format) {
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
    int32_t error = glGetError();
    if (error) {
      XE_CORE_CRITICAL("[GL Error] {} ({})", error, operation);
      return;
    }
  }

#define GLCHECK_STR_STR(A) #A
#define GLCHECK_STR(A) GLCHECK_STR_STR(A)
#define GLCHECK(...) {__VA_ARGS__; checkGLError(__FILE__  ":" GLCHECK_STR(__LINE__) "\n\t-> " #__VA_ARGS__);}

  static void initTextureParams(uint32_t target, const Texture::Info &info) {
    GLCHECK(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, toGL(info.minFilter)));
    GLCHECK(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, toGL(info.magFilter)));
    GLCHECK(glTexParameteri(target, GL_TEXTURE_WRAP_S, toGL(info.wrapS)));
    if (target > static_cast<uint32_t>(TextureType::T1D)) {
      GLCHECK(glTexParameteri(target, GL_TEXTURE_WRAP_T, toGL(info.wrapT)));
    }
    if (target > static_cast<uint32_t>(TextureType::T2D)) {
      GLCHECK(glTexParameteri(target, GL_TEXTURE_WRAP_R, toGL(info.wrapR)));
    }
  }

  static void initTexture(std::pair<TextureInstance *, Backend::Texture *> t) {
    uint32_t id = t.second->texture;
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
          for (uint32_t i = 0; i < 6; ++i) {
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

  static void setupFramebufferTexture(std::pair<TextureInstance *, Backend::Texture *> t, uint32_t mipLevel, uint16_t i) {
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

  static void setupFramebufferDepth(std::pair<TextureInstance *, Backend::Texture *> t, uint32_t mipLevel) {
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

  static std::string prettyCodeError(const char *src) {
    std::stringstream ss;

    uint32_t i = 0;
    uint32_t line = 1;
    char ch = 0;
    ss << line++ << "\t|  ";
    while ((ch = src[i]) != '\0') {
      if (ch == '\n') {
        ss << '\n' << line++ << "\t|  ";
        i++;
      } else {
        ss << src[i++];
      }
    }
    return ss.str();
  }

  static int32_t compileShader(uint32_t type, const char *src) {
    if (src[0] == '\0') {
      return -1;
    }

    int32_t shader = 0;
    GLCHECK(shader = glCreateShader(type));
    if (!shader) {
      XE_CORE_ERROR("[GL Error] Could not create shader program");
      return 0;
    }
    int32_t compiled = 0;
    GLCHECK(glShaderSource(shader, 1, &src, nullptr));
    GLCHECK(glCompileShader(shader));
    GLCHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));

    if (!compiled) {
      static const uint32_t maxLength = 2048;
      char buffer[maxLength];
      GLCHECK(glGetShaderInfoLog(shader, maxLength, nullptr, buffer));
      std::string code = prettyCodeError(src);
      XE_CORE_ERROR("[GL Error] Could compile shader ({}):{}\n// CODE:\n{}", type, buffer, code);
      GLCHECK(glDeleteShader(shader));
      return 0;
    }
    return shader;
  }

  static bool parseOrError(std::string_view source, Backend::Uniform *uniforms, uint32_t &outUsedUniforms, uint32_t &outSize) {
    if (!GLShaderParser::parse(source, uniforms, outUsedUniforms, outSize)) {
      XE_CORE_ERROR("[GL Error] Max shader uniforms reached ({}/{})", cMaxShaderUniforms, outUsedUniforms);
    }
    return true;
  }

  static void setUniform(int32_t loc, VertexFormat::Enum type, const void *data) {
    switch (type) {
      case VertexFormat::Float: {
        GLCHECK(glUniform1f(loc, *reinterpret_cast<const float *>(data)));
        break;
      }
      case VertexFormat::Float2: {
        const float *v = reinterpret_cast<const float *>(data);
        GLCHECK(glUniform2f(loc, v[0], v[1]));
        break;
      }
      case VertexFormat::Float3: {
        const float *v = reinterpret_cast<const float *>(data);
        GLCHECK(glUniform3f(loc, v[0], v[1], v[2]));
        break;
      }
      case VertexFormat::Float4: {
        const float *v = reinterpret_cast<const float *>(data);
        GLCHECK(glUniform4f(loc, v[0], v[1], v[2], v[3]));
        break;
      }
      case VertexFormat::Int32: {
        glUniform1i(loc, *reinterpret_cast<const int32_t *>(data));
        break;
      }
      case VertexFormat::Mat4: {
        GLCHECK(glUniformMatrix4fv(loc, 1, GL_TRUE, reinterpret_cast<const float *>(data)));
        break;
      }
      default: break;
    }
  }

  std::atomic<uint32_t> Backend::drawCalls = 0;

  void Backend::initBackend(Backend **b, const Params::GPU &params) {
    *b = new Backend();

    (*b)->buffers.alloc(params.maxBuffers);
    (*b)->textures.alloc(params.maxTextures);
    (*b)->pipelines.alloc(params.maxPipelines);
    (*b)->framebuffers.alloc(params.maxFramebuffers);
  }

  void Backend::destroyBackend(Backend **b) {
    delete *b;
    *b = nullptr;
  }

  void Backend::clear(const DisplayList::ClearData &d) {
    uint32_t mask = 0;
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
                                           &d.framebuffer.ctx->backend_->framebuffers);
      uint32_t fbId = fb.second->framebuffer;
      if (!fbId) {
        GLCHECK(glGenFramebuffers(1, &fbId));
        fb.second->framebuffer = fbId;
      }

      GLCHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbId));

      if (d.cubemapTarget != CubemapTarget::Invalid) {
        for (uint32_t i = 0; i < fb.first->info.colorAttachmentsSize; ++i) {
          auto tex = RenderContext::getResource(fb.first->colorAttachments[i]->id,
                                                &fb.first->colorAttachments[i]->ctx->textures_,
                                                &fb.first->colorAttachments[i]->ctx->backend_->textures);
          initTexture(tex);
          if (tex.second->target == GL_TEXTURE_CUBE_MAP) {
            GLCHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, toGL(d.cubemapTarget),
                                           tex.second->texture, d.mipLevel));
          }
        }
      } else {
        for (uint16_t i = 0; i < fb.first->info.colorAttachmentsSize; ++i) {
          auto tex = RenderContext::getResource(fb.first->colorAttachments[i]->id,
                                                &fb.first->colorAttachments[i]->ctx->textures_,
                                                &fb.first->colorAttachments[i]->ctx->backend_->textures);
          initTexture(tex);
          setupFramebufferTexture(tex, d.mipLevel, i);
        }

        if (RenderContext::checkValidResource(fb.first->depthAttachment->id, &d.framebuffer.ctx->textures_)) {
          auto tex = RenderContext::getResource(fb.first->depthAttachment->id,
                                                &fb.first->depthAttachment->ctx->textures_,
                                                &fb.first->depthAttachment->ctx->backend_->textures);
          initTexture(tex);
          setupFramebufferDepth(tex, d.mipLevel);
        }
      }

      memory<uint32_t> buffers;
      buffers.alloc(fb.first->info.colorAttachmentsSize);
      uint32_t count = 0;
      for (uint32_t i = 0; i < fb.first->info.colorAttachmentsSize; ++i) {
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
    auto b = RenderContext::getResource(d.buffer.id, &d.buffer.ctx->buffers_, &d.buffer.ctx->backend_->buffers);
    uint32_t id = b.second->buffer;
    const uint32_t target = toGL(b.first->info.bufferType);

    if (!id) {
      GLCHECK(glGenBuffers(1, &id));
      GLCHECK(glBindBuffer(target, id));
      GLCHECK(glBufferData(target, d.size, nullptr, toGL(b.first->info.usage)));
      b.second->buffer = id;
    }

    GLCHECK(glBindBuffer(target, id));

    if (b.first->info.size != d.size) {
      GLCHECK(glBufferData(target, d.size, nullptr, toGL(b.first->info.usage)));
      b.first->info.size = d.size;
    }

    GLCHECK(glBufferSubData(target, d.offset, d.size, d.data));

    if (target == GL_UNIFORM_BUFFER) {
      if (b.first->info.binding != -1) {
        GLCHECK(glBindBufferBase(target, b.first->info.binding, id));
      } else {
        XE_CORE_ERROR("[GL Error] Unable to bind uniform buffer block '{}', id = -1", b.first->info.name);
      }
    }
  }

  void Backend::fillTexture(DisplayList::FillTextureData &d) {
    auto t = RenderContext::getResource(d.texture.id, &d.texture.ctx->textures_, &d.texture.ctx->backend_->textures);

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
        GLCHECK(glTexSubImage1D(GL_TEXTURE_1D, 0, d.offsetX, d.width, backEnd.format, backEnd.type,
                                !d.data0 ? Embedded::defaultTextureData() : d.data0));
        break;
      }
      case TextureType::T2D: {
        GLCHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, d.offsetX, d.offsetY, d.width, d.height, backEnd.format,
                                backEnd.type, !d.data0 ? Embedded::defaultTextureData() : d.data0));
        break;
      }
      case TextureType::T3D: {
        GLCHECK(glTexSubImage3D(GL_TEXTURE_3D, 0, d.offsetX, d.offsetY, d.offsetZ, d.width, d.height, d.depth,
                                backEnd.format, backEnd.type, !d.data0 ? Embedded::defaultTextureData() : d.data0));
        break;
      }
      case TextureType::CubeMap: {
        const void *data[6] = {d.data0, d.data1, d.data2, d.data3, d.data4, d.data5};
        for (uint32_t i = 0; i < 6; ++i) {
          GLCHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, d.offsetX, d.offsetY, d.width, d.height,
                                  backEnd.format, backEnd.type, !data[i] ? Embedded::defaultTextureData() : data[i]));
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
                                          &d.pipeline.ctx->backend_->pipelines);
    if (mat.second->program == -1) {
      mat.first->info.renderMode = RenderMode::Disabled;
      return;
    }

    if (lastPipelineChanged) {
      //shader creation
      if (mat.second->program == 0) {
        for (int32_t &i : mat.second->textureUniformsLoc) {
          i = -1;
        }

        bool success = true;

        const int32_t shaderV = compileShader(GL_VERTEX_SHADER, mat.first->vertShader.c_str());
        const int32_t shaderF = compileShader(GL_FRAGMENT_SHADER, mat.first->fragShader.c_str());
        const int32_t shaderTC = compileShader(GL_TESS_CONTROL_SHADER, mat.first->tessControlShader.c_str());
        const int32_t shaderTE = compileShader(GL_TESS_EVALUATION_SHADER, mat.first->tessEvalShader.c_str());
        const int32_t shaderG = compileShader(GL_GEOMETRY_SHADER, mat.first->geomShader.c_str());
        if (!shaderV || !shaderF || !shaderTC || !shaderTE || !shaderG) {
          success = false;
        }

        uint32_t programId = 0;
        if (success) {
          GLCHECK(programId = glCreateProgram());
          if (!programId) {
            XE_CORE_ERROR("[GL Error] Could not create shader program");
            success = false;
          }
        }

        uint32_t bufferSize = 0;
        if (success && shaderV != -1) {
          GLCHECK(glAttachShader(programId, shaderV));
          if (!parseOrError(mat.first->vertShader, mat.second->uniforms, mat.second->usedUniforms, bufferSize)) {
            success = false;
          }
        }
        if (success && shaderF != -1) {
          GLCHECK(glAttachShader(programId, shaderF));
          if (!parseOrError(mat.first->fragShader, mat.second->uniforms, mat.second->usedUniforms, bufferSize)) {
            success = false;
          }
        }
        if (success && shaderTC != -1) {
          GLCHECK(glAttachShader(programId, shaderTC));
          if (!parseOrError(mat.first->tessControlShader, mat.second->uniforms, mat.second->usedUniforms, bufferSize)) {
            success = false;
          }
        }
        if (success && shaderTE != -1) {
          GLCHECK(glAttachShader(programId, shaderTE));
          if (!parseOrError(mat.first->tessEvalShader, mat.second->uniforms, mat.second->usedUniforms, bufferSize)) {
            success = false;
          }
        }
        if (success && shaderG != -1) {
          GLCHECK(glAttachShader(programId, shaderG));
          if (!parseOrError(mat.first->geomShader, mat.second->uniforms,
                            mat.second->usedUniforms, bufferSize)) {
            success = false;
          }
        }

        if (success) {
          GLCHECK(glLinkProgram(programId));
          int32_t linkStatus = GL_FALSE;
          GLCHECK(glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus));
          if (linkStatus != GL_TRUE) {
            char log[2048];
            GLCHECK(glGetProgramInfoLog(programId, 2048, nullptr, log));
            GLCHECK(glDeleteProgram(programId));
            XE_CORE_ERROR("[GL Error] Could not link program:\n\t{}", log);
            success = false;
          }
        }

        if (shaderV != -1) {
          GLCHECK(glDeleteShader(shaderV));
        }
        if (shaderF != -1) {
          GLCHECK(glDeleteShader(shaderF));
        }
        if (shaderTC != -1) {
          GLCHECK(glDeleteShader(shaderTC));
        }
        if (shaderTE != -1) {
          GLCHECK(glDeleteShader(shaderTE));
        }
        if (shaderG != -1) {
          GLCHECK(glDeleteShader(shaderG));
        }

        if (success) {
          mat.second->uniformData.alloc(bufferSize);

          for (auto i = 0; i < cMaxVertexAttribs; ++i) {
            const auto &attrib = mat.first->info.attribs[i];
            if (attrib.format) {
              GLCHECK(glBindAttribLocation(programId, i, attrib.name));
            } else {
              break;
            }
          }
        } else {
          mat.second->program = -1;
          mat.first->info.renderMode = RenderMode::Disabled;
          return;
        }

        mat.second->program = programId;

        //get texture locations
        for (uint32_t i = 0; i < cMaxTextureUnits; ++i) {
          char name[32];
          switch (mat.first->info.textures[i]) {
            case TextureType::T1D: snprintf(name, 32, "u_tex1d%i", i);
              break;
            case TextureType::T2D: snprintf(name, 32, "u_tex2d%i", i);
              break;
            case TextureType::T3D: snprintf(name, 32, "u_tex3d%i", i);
              break;
            case TextureType::CubeMap: snprintf(name, 32, "u_texCube%i", i);
              break;
            default: break;
          }

          if (static_cast<uint32_t>(mat.first->info.textures[i])) {
            int32_t loc = -1;
            GLCHECK(loc = glGetUniformLocation(programId, name));
            if (loc != -1) {
              mat.second->textureUniformsLoc[i] = loc;
            } else {
              XE_CORE_ERROR("[GL Error] Unable to find texture uniform '{}' (pipeline {})", name, d.pipeline.id);
            }
          }
        }

        auto &&p = mat.second;
        for (uint32_t i = 0; i < p->usedUniforms; ++i) {
          GLCHECK(p->uniforms[i].loc = glGetUniformLocation(p->program, p->uniforms[i].name.c_str()));
        }

      } //shader creation

      GLCHECK(glUseProgram(mat.second->program));

      if (mat.first->info.blend.enabled) {
        GLCHECK(glEnable(GL_BLEND));
        const vec4 &c = mat.first->info.blend.color;
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

      const uint8_t colorWrite = (mat.first->info.rgbaWrite) ? GL_TRUE : GL_FALSE;
      const uint8_t depthWrite = (mat.first->info.depthWrite) ? GL_TRUE : GL_FALSE;
      GLCHECK(glColorMask(colorWrite, colorWrite, colorWrite, colorWrite));
      GLCHECK(glDepthMask(depthWrite));

      if (mat.first->info.depthFunc != CompareFunc::Disabled) {
        GLCHECK(glDepthFunc(toGL(mat.first->info.depthFunc)));
        GLCHECK(glEnable(GL_DEPTH_TEST));
      } else {
        GLCHECK(glDisable(GL_DEPTH_TEST));
      }

      for (uint32_t i = 0; i < cMaxVertexAttribs; ++i) {
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

      for (uint32_t i = 0; i < cMaxUniformBuffers; ++i) {
        if (d.uniformBuffer[i].id) {
          auto ubo = RenderContext::getResource(d.uniformBuffer[i].id, &d.pipeline.ctx->buffers_, &d.pipeline.ctx->backend_->buffers);
          auto uniformIndex = glGetUniformBlockIndex(mat.second->program, ubo.first->info.name.data());
          GLCHECK(glUniformBlockBinding(mat.second->program, uniformIndex, ubo.first->info.binding));
          if (uniformIndex == GL_INVALID_INDEX) {
            XE_CORE_ERROR("[GL Error] Unable to find uniform block '{}'. Invalid index.", ubo.first->info.name);
          }
        }
      }
    }//lastPipelineChanged

    //uniformfs setup
    for (auto &&u : d.uniform) {
      if (!u.name) {
        break;
      }
      for (uint32_t i = 0; i < mat.second->usedUniforms; ++i) {
        auto &&mu = mat.second->uniforms[i];
        if (mu.name == u.name) {
          std::memcpy(mat.second->uniformData.data.get() + mu.offset, u.data, mu.size);
          setUniform(mu.loc, mu.type, mat.second->uniformData.data.get() + mu.offset);
          break;
        }
      }
    }

    if (d.scissor[2] > 0.0f && d.scissor[3] > 0.0f) {
      GLCHECK(glScissor(static_cast<int32_t>(d.scissor[0]),
                        static_cast<int32_t>(d.scissor[1]),
                        static_cast<int32_t>(d.scissor[2]),
                        static_cast<int32_t>(d.scissor[3])));
      GLCHECK(glEnable(GL_SCISSOR_TEST));
    } else {
      GLCHECK(glDisable(GL_SCISSOR_TEST));
    }
  }

  void Backend::render(DisplayList::RenderData &d) {
    auto buf = RenderContext::getResource(d.indexBuffer.id, &d.indexBuffer.ctx->buffers_,
                                          &d.indexBuffer.ctx->backend_->buffers);

    auto mat = RenderContext::getResource(d.indexBuffer.ctx->lastPipeline_.pipeline.id, &d.indexBuffer.ctx->pipelines_,
                                          &d.indexBuffer.ctx->backend_->pipelines);

    if (mat.first->info.renderMode == RenderMode::Disabled) {
      return;
    }

    if (!buf.second->buffer) {
      XE_CORE_ERROR("[GL Error] Invalid index buffer");
    }

    auto &pipeline = d.indexBuffer.ctx->lastPipeline_;

    uint32_t texUnit = 0;
    for (uint32_t i = 0; i < cMaxTextureUnits; ++i) {
      if (mat.second->textureUniformsLoc[i] >= 0) {
        auto tex = RenderContext::getResource(pipeline.texture[i].id, &d.indexBuffer.ctx->textures_,
                                              &d.indexBuffer.ctx->backend_->textures);
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
      const uint32_t attribFormat = mat.first->info.attribs[i].format;
      if (attribFormat) {
        const uint32_t bufferIndex = mat.first->info.attribs[i].bufferIndex;
        const uint32_t bufferId = pipeline.buffer[bufferIndex].id;
        if (!bufferId) {
          XE_CORE_ERROR("[GL Error] Expected valid buffer (see pipeline declaration)");
          return;
        }
        auto buffer = RenderContext::getResource(bufferId, &d.indexBuffer.ctx->buffers_,
                                                 &d.indexBuffer.ctx->backend_->buffers);
        if (buffer.second->buffer == 0) {
          XE_CORE_ERROR("[GL Error] Invalid GL buffer (vertex data)");
        }

        GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer.second->buffer));
        const int32_t attrSize = (attribFormat & VertexFormat::NumComponentsMask) >> VertexFormat::NumComponentsShift;
        const uint32_t attrType = vertexTypeToGL(attribFormat);
        const uint8_t attrNormalized = (attribFormat & VertexFormat::Normalized) ? GL_TRUE : GL_FALSE;
        const int32_t attrStride = mat.first->info.attribs[i].stride;
        const int32_t attrOffset = mat.first->info.attribs[i].offset;
        GLCHECK(glVertexAttribPointer(i, attrSize, attrType, attrNormalized, attrStride, reinterpret_cast<void *>((size_t) attrOffset)));
        GLCHECK(glEnableVertexAttribArray(i));
      } else {
        break;
      }
    }

    switch (mat.first->info.renderMode) {
      case RenderMode::Solid: {
        GLCHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(toGL(mat.first->info.primitive),
                                        d.count, toGL(d.type), reinterpret_cast<void *>((size_t) d.offset), d.instances));
        ++drawCalls;
        break;
      }
      case RenderMode::Wireframe: {
        GLCHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(toGL(mat.first->info.primitive),
                                        d.count, toGL(d.type), reinterpret_cast<void *>((size_t) d.offset), d.instances));
        ++drawCalls;
        break;
      }
      default: break;
    }
  }

  void Backend::destroy(DisplayList::DestroyData &d) {
    if (!d.resource.ctx && !d.resource.id) {
      return;
    }

    const int32_t pos = RenderContext::index(d.resource.id);
    XE_CORE_INFO("[GL Backend] Destroy resource '{}' ({} -> {})", d.resource.type, d.resource.id, pos);
    destroyResource(d.resource.ctx, d.resource.type, pos);
    switch (d.resource.type) {
      case Resource::ResourceType::Buffer: {
        d.resource.ctx->buffers_[pos].state = 0;
        break;
      }
      case Resource::ResourceType::Pipeline: {
        d.resource.ctx->pipelines_[pos].state = 0;
        break;
      }
      case Resource::ResourceType::Texture: {
        d.resource.ctx->textures_[pos].state = 0;
        break;
      }
      case Resource::ResourceType::Framebuffer: {
        FramebufferInstance *fb = &d.resource.ctx->framebuffers_[pos];

        for (auto &colorAttachment : fb->colorAttachments) {
          if (colorAttachment) {
            const uint32_t texPos = RenderContext::index(colorAttachment->id);
            XE_CORE_INFO("[GL Backend] Destroy framebuffer color texture ({} -> {})", colorAttachment->id, texPos);
            destroyResource(d.resource.ctx, Resource::ResourceType::Texture, texPos);
            d.resource.ctx->textures_[texPos].state = 0;
          }
        }
        if (RenderContext::checkValidResource(fb->depthAttachment->id, &d.resource.ctx->textures_)) {
          if (fb->depthAttachment) {
            const uint32_t texPos = RenderContext::index(fb->depthAttachment->id);
            XE_CORE_INFO("[GL Backend] Destroy framebuffer depth texture ({} -> {})", fb->depthAttachment->id, texPos);
            destroyResource(d.resource.ctx, Resource::ResourceType::Texture, texPos);
            d.resource.ctx->textures_[texPos].state = 0;
          }
        }
        fb->state = 0;
        break;
      }
      default:
      case Resource::ResourceType::Invalid: {
        XE_CORE_WARN("[GL Backend] Trying to destroy an invalid resource");
        break;
      }
    }
  }

  void Backend::destroyResource(RenderContext *ctx, Resource::ResourceType type, uint32_t pos) {
    Backend *b = ctx->backend_;
    switch (type) {
      case Resource::ResourceType::Buffer: {
        GLCHECK(glDeleteBuffers(1, &b->buffers[pos].buffer));
        b->buffers[pos].buffer = 0;
        Engine::ref().gpu().usedBuffers_--;
        break;
      }
      case Resource::ResourceType::Pipeline: {
        GLCHECK(glDeleteProgram(b->pipelines[pos].program));
        b->pipelines[pos].program = 0;
        Engine::ref().gpu().usedPipelines_--;
        break;
      }
      case Resource::ResourceType::Texture: {
        GLCHECK(glDeleteTextures(1, &b->textures[pos].texture));
        b->textures[pos].texture = 0;
        Engine::ref().gpu().usedTextures_--;
        break;
      }
      case Resource::ResourceType::Framebuffer: {
        GLCHECK(glDeleteFramebuffers(1, &b->framebuffers[pos].framebuffer));
        b->framebuffers[pos].framebuffer = 0;
        Engine::ref().gpu().usedFramebuffers_--;
        break;
      }
      default:
      case Resource::ResourceType::Invalid: {
        XE_CORE_WARN("[GL Backend] Trying to destroy an invalid resource");
        break;
      }
    }
  }

}
