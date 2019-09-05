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

  static uint toGL(SamplerWrapping e) {
    switch (e) {
      case SamplerWrapping::Repeat: return GL_REPEAT;
      case SamplerWrapping::MirroredRepeat: return GL_MIRRORED_REPEAT;
      case SamplerWrapping::Clamp:
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
      default: return GL_FALSE; //error
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
      default: return GL_FALSE; //error
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

  static void initTexture(std::pair<TextureInstance *, BackEnd::Texture *> t, bool forceUpdate = false) {
    uint id = t.second->texture;
    auto &backEnd = t.second;
    if (!id) {
      GLCHECK(glGenTextures(1, &id));
      backEnd->texture = id;
      t.first->id = id;
      forceUpdate = true;
    }
    if (forceUpdate) {
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

  void fillTexture(DrawList::FillTextureData &d) {
    auto t = RenderContext::getResource(d.texture.id, &d.texture.ctx->textures_, &d.texture.ctx->backEnd_->textures);

    bool sizeChanged = false;

    d.width = d.width ? d.width : t.first->info.width;
    d.height = d.height ? d.height : t.first->info.height;
    d.depth = d.depth ? d.depth : t.first->info.depth;
    if (d.width != t.first->info.width || d.height != t.first->info.height || d.depth != t.first->info.depth) {
      t.first->info.width = d.width;
      t.first->info.height = d.height;
      t.first->info.depth = d.depth;
      sizeChanged = true;
    }

    initTexture(t, sizeChanged);
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

  static uint compileShader(uint type, const char *src) {
    uint shader = glCreateShader(type);
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
      XE_CORE_ERROR("[GL Error] Could compile shader ({}):{} // CODE: {}", type, buffer, src);
      GLCHECK(glDeleteShader(shader));
      return 0;
    }
    return shader;
  }

  void setupMaterial(DrawList::SetupMaterialData &d) {
    bool mainMaterialChanged = d.material.id != d.material.ctx->mainMaterial_.material.id;
    d.material.ctx->mainMaterial_ = d;
    auto mat = RenderContext::getResource(d.material.ctx->mainMaterial_.material.id, &d.material.ctx->materials_,
                                          &d.material.ctx->backEnd_->materials);
    if (mainMaterialChanged) {
      if (mat.second->program == 0) {
        for (int32 &i : mat.second->textureUniformsLoc) {
          i = -1;
        }

        uint shaderV = compileShader(GL_VERTEX_SHADER, mat.first->vertShader.c_str());
        uint shaderF = compileShader(GL_FRAGMENT_SHADER, mat.first->fragShader.c_str());
        if (!shaderV || !shaderF) {
          return;
        }

        uint programId = glCreateProgram();
        if (!programId) {
          XE_CORE_ERROR("[GL Error] Could not create shader program");
          return;
        }
        GLCHECK(glAttachShader(programId, shaderV));
        GLCHECK(glAttachShader(programId, shaderF));

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

        GLCHECK(glDeleteShader(shaderV));
        GLCHECK(glDeleteShader(shaderF));

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
            mat.second->textureUniformsLoc[i] = glGetUniformLocation(programId, name);
          }
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

      byte rgbw = (mat.first->info.rgbaWrite) ? GL_TRUE : GL_FALSE;
      byte dephtw = (mat.first->info.depthWrite) ? GL_TRUE : GL_FALSE;
      GLCHECK(glColorMask(rgbw, rgbw, rgbw, rgbw));
      GLCHECK(glDepthMask(dephtw));

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
          auto ubo = RenderContext::getResource(d.uniformBuffer[i].id, &d.material.ctx->buffers_,
                                                &d.material.ctx->backEnd_->buffers);
          uint uniformIndex = glGetUniformBlockIndex(mat.second->program, ubo.first->info.name_);
          GLCHECK(glUniformBlockBinding(mat.second->program, uniformIndex, ubo.second->buffer));
        }
      }
    }

    glUniformMatrix4fv(glGetUniformLocation(mat.second->program, cShaderModelUniform), 1,
                       GL_TRUE, (const float *) &d.modelMatrix);

    if (d.scissor[2] > 0.0f && d.scissor[3] > 0.0f) {
      GLCHECK(glScissor((int32) d.scissor[0], (int32) d.scissor[1], (int32) d.scissor[2], (int32) d.scissor[3]));
      GLCHECK(glEnable(GL_SCISSOR_TEST));
    } else {
      GLCHECK(glDisable(GL_SCISSOR_TEST));
    }
  }

  void render(DrawList::RenderData &d) {
    auto buf = RenderContext::getResource(d.indexBuffer.id, &d.indexBuffer.ctx->buffers_,
                                          &d.indexBuffer.ctx->backEnd_->buffers);

    auto mat = RenderContext::getResource(d.indexBuffer.ctx->mainMaterial_.material.id, &d.indexBuffer.ctx->materials_,
                                          &d.indexBuffer.ctx->backEnd_->materials);

    if (mat.first->info.renderMode == RenderMode::Disabled) {
      return;
    }

    if (!buf.second->buffer) {
      XE_CORE_ERROR("[GL Error] Invalid Index buffer");
    }

    auto &mainMaterial = d.indexBuffer.ctx->mainMaterial_;

    size_t texUnit = 0;
    for (auto i = 0; i < cMaxTextureUnits; ++i) {
      if (mat.second->textureUniformsLoc[i] >= 0) {
        auto tex = RenderContext::getResource(mainMaterial.texture[i].id, &d.indexBuffer.ctx->textures_,
                                              &d.indexBuffer.ctx->backEnd_->textures);
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
      uint attribFormat = mat.first->info.attribs[i].format;
      if (attribFormat) {
        size_t bufferIndex = mat.first->info.attribs[i].bufferIndex;
        size_t bufferId = mainMaterial.buffer[bufferIndex].id;
        if (!bufferId) {
          XE_CORE_ERROR("[GL Error] Expected Valid buffer (see pipeline declaration)");
          return;
        }
        auto buffer = RenderContext::getResource(bufferId, &d.indexBuffer.ctx->buffers_,
                                                 &d.indexBuffer.ctx->backEnd_->buffers);
        if (buffer.second->buffer == 0) {
          XE_CORE_ERROR("[GL Error] Invalid GL buffer (vertex data)");
        }

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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(toGL(mat.first->info.primitive),
                                        d.count, toGL(d.type), (void *) d.offset, d.instances));
        break;
      }
      case RenderMode::Wireframe: {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(toGL(mat.first->info.primitive),
                                        d.count, toGL(d.type), (void *) d.offset, d.instances));
        break;
      }
      case RenderMode::SolidWireframe: {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(toGL(mat.first->info.primitive),
                                        d.count, toGL(d.type), (void *) d.offset, d.instances));
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(toGL(mat.first->info.primitive),
                                        d.count, toGL(d.type), (void *) d.offset, d.instances));
        break;
      }
      default: break;
    }

  }

}
