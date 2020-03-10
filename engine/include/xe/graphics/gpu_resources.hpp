//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_GPU_RESOURCES_HPP
#define XE_GPU_RESOURCES_HPP

#include <xe/common.hpp>

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

      RenderContext *ctx;
      ResourceType type;
      uint32_t id;
    };

    struct Buffer : public Resource {
      Buffer(RenderContext *ctx = nullptr, uint32_t id = 0) : Resource{ctx, ResourceType::Buffer, id} { }
      struct Info {
        BufferType type_;
        Usage usage_;
        uint32_t size_;
        const char *name_;
      };
    };

    struct XE_API Texture : public Resource {
      Texture(RenderContext *ctx = nullptr, uint32_t id = 0) : Resource{ctx, ResourceType::Texture, id} { }
      struct Info {
        uint16_t width = 1;
        uint16_t height = 1;
        uint16_t depth = 1;
        TextureMinFilter minFilter = TextureMinFilter::Linear;
        TextureMagFilter magFilter = TextureMagFilter::Linear;
        TextureWrap wrapping[3] = {TextureWrap::Clamp, TextureWrap::Clamp, TextureWrap::Clamp};
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
        Texture::Info depthStencilAttachmentInfo;
        uint16_t colorAttachmentsSize;
        vec2u size;
      };

      Texture colorAttachment(uint16_t index = 0) const;
      Texture depthStencilAttachment() const;

      void setColorAttachment(Texture t, uint16_t index = 0);
      void setDepthStencilAttachment(Texture t);
    };

  }

}

#endif //XE_GPU_RESOURCES_HPP
