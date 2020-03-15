//
// Created by trbflxr on 3/14/2020.
//

#ifndef XE_COMPOSER_HPP
#define XE_COMPOSER_HPP

#include <xe/core/object.hpp>
#include <xe/graphics/gpu_resources.hpp>

namespace xe {

  class XE_API Composer : public Object {
  XE_OBJECT(Composer, Object);
  public:
    explicit Composer();

    void init(const vec2u &size, TexelsFormat format = TexelsFormat::Rgba16f);

    void present() const;

    const vec2u &viewport() const { return size_; }

    const gpu::Framebuffer &framebuffer() const { return framebuffer_; }
    gpu::Texture targetTexture() const { return framebuffer_.colorAttachment(0); }
    gpu::Texture targetDepth() const { return framebuffer_.depthStencilAttachment(); }

  private:
    vec2u size_;
    TexelsFormat format_ = TexelsFormat::None;

    gpu::Framebuffer framebuffer_;

    struct {
      xe::gpu::Pipeline material;
      xe::gpu::Buffer vertexBuffer;
      xe::gpu::Buffer indexBuffer;
    } quad_;
  };

}

#endif //XE_COMPOSER_HPP