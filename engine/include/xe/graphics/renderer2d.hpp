//
// Created by trbflxr on 3/15/2020.
//

#ifndef XE_RENDERER2D_HPP
#define XE_RENDERER2D_HPP

#include <xe/core/object.hpp>
#include <xe/graphics/camera.hpp>
#include <xe/graphics/texture.hpp>
#include <xe/math/rect.hpp>

namespace xe {

  class XE_API Renderer2d : public Object {
  XE_OBJECT(Renderer2d, Object);
  public:
    struct VertexData {
      vec2 position;
      vec2 texCoords;
      Color color;
    };

    struct CameraData {
      mat4 proj;
      mat4 view;
    };

  public:
    explicit Renderer2d(Camera &camera);

    void submit(const vec2 &pos, const vec2 &size, Color color,
                const std::shared_ptr<Texture> &texture,
                const rect &textureArea01 = {0.0f, 0.0f, 1.0f, 1.0f});

    void begin();
    void end();
    void flush();

    void setCamera(Camera &camera) { camera_ = camera; }

    Camera &camera() { return camera_; }
    const Camera &camera() const { return camera_; }
    Transform &transform() { return transform_; }

  private:
    void init();
    void initArrays();
    void initPipeline();
    void initBuffers();

  private:
    Camera &camera_;
    Transform transform_;

    uint32_t maxInstances_ = 50'000;

    uint32_t verticesCount_ = 0;
    uint32_t indicesCount_ = 0;

    uint32_t verticesSize_ = 0;
    uint32_t instancesSize_ = 0;
    uint32_t verticesBufferSize_ = 0;
    uint32_t instancesBufferSize_ = 0;

    VertexData *buffer_;
    std::unique_ptr<VertexData[]> bufferData_;
    std::unique_ptr<uint32_t[]> indices_;

    std::shared_ptr<Texture> activeTexture_;

    CameraData cameraData_;

    gpu::Pipeline pipeline_;
    gpu::Buffer vertexBuffer_;
    gpu::Buffer indexBuffer_;
    gpu::Buffer uniformBuffer_;
  };

}

#endif //XE_RENDERER2D_HPP
