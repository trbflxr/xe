//
// Created by trbflxr on 3/14/2020.
//

#ifndef XE_COMPOSER_HPP
#define XE_COMPOSER_HPP

#include <xe/core/object.hpp>
#include <xe/graphics/gpu_resource.hpp>
#include <xe/graphics/gpu_resources.hpp>

namespace xe {

  class XE_API Composer : public Object, GPUResource {
  XE_OBJECT(Composer, Object);
  public:
    explicit Composer();

    void init(const vec2u &size, TexelsFormat format = TexelsFormat::Rgba16f);

    void destroy() override;

    void present() const;

    const vec2u &viewport() const { return size_; }

    const gpu::Framebuffer &framebuffer() const { return *framebuffer_; }
    gpu::Texture targetTexture() const { return framebuffer_->colorAttachment(0); }
    gpu::Texture targetDepth() const { return framebuffer_->depthStencilAttachment(); }

  private:
    vec2u size_;
    TexelsFormat format_ = TexelsFormat::None;

    std::shared_ptr<gpu::Framebuffer> framebuffer_;

    struct {
      std::shared_ptr<xe::gpu::Pipeline> pipeline;
      std::shared_ptr<xe::gpu::Buffer> vertexBuffer;
      std::shared_ptr<xe::gpu::Buffer> indexBuffer;
    } quad_;
  };

}

#endif //XE_COMPOSER_HPP
