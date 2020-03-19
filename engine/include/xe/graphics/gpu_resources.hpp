//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_GPU_RESOURCES_HPP
#define XE_GPU_RESOURCES_HPP

#include <xe/math/math.hpp>
#include <xe/graphics/types.hpp>

namespace xe {

  class RenderContext;

  namespace gpu {

    struct Resource {
      enum class ResourceType {
        Invalid,
        Buffer,
        Texture,
        Pipeline,
        Framebuffer
      };

      template<typename OStream>
      friend OStream &operator<<(OStream &os, Resource::ResourceType t) {
        switch (t) {
          case Resource::ResourceType::Buffer: return os << "Buffer";
          case Resource::ResourceType::Texture: return os << "Texture";
          case Resource::ResourceType::Pipeline: return os << "Pipeline";
          case Resource::ResourceType::Framebuffer: return os << "Framebuffer";
          default: return os << "Invalid";
        }
      }

      RenderContext *ctx = nullptr;
      ResourceType type = ResourceType::Invalid;
      uint32_t id = 0;
    };

    struct Buffer : public Resource {
      Buffer(RenderContext *ctx = nullptr, uint32_t id = 0) : Resource{ctx, ResourceType::Buffer, id} { }
      struct Info {
        BufferType bufferType = BufferType::Invalid;
        Usage usage = Usage::Static;
        uint32_t size = 0;
        std::string_view name;
        int32_t binding = -1;
      };
    };

    struct XE_API Texture : public Resource {
      Texture(RenderContext *ctx = nullptr, uint32_t id = 0) : Resource{ctx, ResourceType::Texture, id} { }
      struct Info {
        uint32_t width = 1;
        uint32_t height = 1;
        uint32_t depth = 1;
        TextureMinFilter minFilter = TextureMinFilter::Linear;
        TextureMagFilter magFilter = TextureMagFilter::Linear;
        TextureWrap wrapS = TextureWrap::Clamp;
        TextureWrap wrapT = TextureWrap::Clamp;
        TextureWrap wrapR = TextureWrap::Clamp;
        TexelsFormat format = TexelsFormat::None;
        Usage usage = Usage::Static;
        TextureType type = TextureType::T2D;
      };

      static void *loadFromFile(std::string_view file, Texture::Info &tex, bool flip = true);
      static std::vector<void *> loadCubemapFromFile(std::string_view rt, std::string_view lf, std::string_view up,
                                                     std::string_view dn, std::string_view bk, std::string_view ft,
                                                     Texture::Info &tex, bool flip = true);
    };

    struct Pipeline : public Resource {
      Pipeline(RenderContext *ctx = nullptr, uint32_t id = 0) : Resource{ctx, ResourceType::Pipeline, id} { }
      struct Info {
        struct Shader {
          std::string vert;
          std::string tessControl;
          std::string tessEval;
          std::string geom;
          std::string frag;
        } shader;
        VertexDeclaration attribs[cMaxVertexAttribs] = { };
        TextureType textures[cMaxTextureUnits] = { };
        Primitive primitive = Primitive::Triangles;
        Cull cull = Cull::Back;
        RenderMode renderMode = RenderMode::Solid;
        struct {
          BlendFactor srcRgb = BlendFactor::SrcAlpha;
          BlendFactor dstRgb = BlendFactor::OneMinusSrcAlpha;
          BlendOp opRgb = BlendOp::Add;
          BlendFactor srcAlpha = BlendFactor::SrcAlpha;
          BlendFactor dstAlpha = BlendFactor::OneMinusSrcAlpha;
          BlendOp opAlpha = BlendOp::Add;
          vec4 color = {0.0f, 0.0f, 0.0f, 0.0f};
          bool enabled = false;
        } blend;
        CompareFunc depthFunc = CompareFunc::Less;
        bool rgbaWrite = true;
        bool depthWrite = true;
      };
    };

    struct XE_API Framebuffer : public Resource {
      Framebuffer(RenderContext *ctx = nullptr, uint32_t id = 0) : Resource{ctx, ResourceType::Framebuffer, id} { }
      struct Info {
        Texture::Info colorAttachmentInfo[cMaxFramebufferColorAttachments];
        Texture::Info depthStencilAttachmentInfo{ };
        uint16_t colorAttachmentsSize = 0;
        vec2u size{ };
      };

      Texture colorAttachment(uint32_t index = 0) const;
      Texture depthStencilAttachment() const;

      void setColorAttachment(std::shared_ptr<Texture> &&t, uint32_t index = 0);
      void setDepthStencilAttachment(std::shared_ptr<Texture> &&t);
    };

  }

}

#endif //XE_GPU_RESOURCES_HPP
