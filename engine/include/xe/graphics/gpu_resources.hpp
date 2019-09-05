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
        Material,
        Framebuffer
      };

      RenderContext *ctx;
      ResourceType type;
      uint id;
    };

    struct Buffer : public Resource {
      Buffer(RenderContext *ctx = nullptr, uint id = 0) : Resource{ctx, ResourceType::Buffer, id} { }
      struct Info {
        BufferType type_;
        Usage usage_;
        uint size_;
        const char *name_;
      };
    };

    struct XE_API Texture : public Resource {
      Texture(RenderContext *ctx = nullptr, uint id = 0) : Resource{ctx, ResourceType::Texture, id} { }
      struct Info {
        uint16 width = 1;
        uint16 height = 1;
        uint16 depth = 1;
        TextureMinFilter minFilter = TextureMinFilter::Linear;
        TextureMagFilter magFilter = TextureMagFilter::Linear;
        SamplerWrapping wrapping[3] = {SamplerWrapping::Repeat, SamplerWrapping::Repeat, SamplerWrapping::Repeat};
        TexelsFormat format = TexelsFormat::None;
        Usage usage = Usage::Static;
        TextureType type = TextureType::T2D;
      };

      static void *loadFromFile(const char *file, Texture::Info &tex, bool flip = true);
      static vector<void *> loadCubemapFromFile(const string &rt, const string &lf, const string &up,
                                                const string &dn, const string &bk, const string &ft,
                                                Texture::Info &tex, bool flip = true);
    };

    struct Material : public Resource {
      Material(RenderContext *ctx = nullptr, uint id = 0) : Resource{ctx, ResourceType::Material, id} { }
      struct Info {
        struct Shader {
          string vert;
          string frag;
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
      Framebuffer(RenderContext *ctx = nullptr, uint id = 0) : Resource{ctx, ResourceType::Framebuffer, id} { }
      struct Info {
        Texture::Info colorAttachmentInfo[cMaxFramebufferColorAttachments];
        Texture::Info depthStencilAttachmentInfo;
        uint16 colorAttachmentsSize;
        vec2 size;
      };

      Texture colorAttachment(uint16 index = 0) const;
      Texture depthStencilAttachment() const;

      void setColorAttachment(Texture t, uint16 index = 0);
      void setDepthStencilAttachment(Texture t);
    };

  }

}

#endif //XE_GPU_RESOURCES_HPP
