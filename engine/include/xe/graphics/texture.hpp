//
// Created by trbflxr on 3/15/2020.
//

#ifndef XE_TEXTURE_HPP
#define XE_TEXTURE_HPP

#include <xe/core/object.hpp>
#include <xe/graphics/color.hpp>
#include <xe/graphics/gpu_resources.hpp>

namespace xe {

  class XE_API Texture : public Object {
  XE_OBJECT(Texture, Object);
  public:
    Texture();
    explicit Texture(const gpu::Texture::Info &info);
    explicit Texture(std::string_view file, gpu::Texture::Info info);
    ~Texture() override;

    bool setup();
    void destroy();

    bool loadFromFile(std::string_view file);

    void setInfo(const gpu::Texture::Info &info);
    void setSize(uint32_t width = 1, uint32_t height = 1, uint32_t depth = 1);
    void setOffset(uint32_t x = 0, uint32_t y = 0, uint32_t z = 0);
    void setFiltering(TextureMinFilter min, TextureMagFilter mag);
    void setWrapping(TextureWrap wrap);
    void setWrapping(TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR);
    void setTexelsFormat(TexelsFormat format);
    void setUsage(Usage usage);
    void setType(TextureType type);
    void setBuildMipmap(bool buildMipmap);
    void setData(void *data, uint32_t index = 0);
    void setData(Color color, uint32_t index = 0);
    void setHdr(bool hdr);

    vec2u size() const { return {gpu_.info.width, gpu_.info.height}; }
    uint32_t depth() const { return gpu_.info.depth; }
    TextureType textureType() const { return gpu_.info.type; }

    bool hasChanged() const { return dirty_; }
    bool loading() const { return loading_; }
    void *data(uint32_t index = 0) const;
    bool hdr() const { return hdr_; }

    const gpu::Texture &raw() const { return *gpu_.tex; }

    uint32_t textureId();
    std::string_view file() const { return file_; }

  private:
    std::string file_ = "";
    bool hdr_ = false;
    bool dirty_ = true;
    bool loading_ = false;
    void *data_[6] = {nullptr};

    uint32_t internalId_ = 0;

    struct {
      std::shared_ptr<gpu::Texture> tex;
      gpu::Texture::Info info{ };
      uint32_t offset[3] = {0};
      bool buildMipmap = false;
    } gpu_;
  };

}

#endif //XE_TEXTURE_HPP
