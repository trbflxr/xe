//
// Created by trbflxr on 3/26/2020.
//

#ifndef XE_TEXT_RENDERER_HPP
#define XE_TEXT_RENDERER_HPP

#include <xe/graphics/text.hpp>
#include <xe/graphics/camera.hpp>

namespace xe {

  class XE_API TextRenderer : public Object, GPUResource {
  XE_OBJECT(TextRenderer, Object);
  public:
    explicit TextRenderer(uint32_t screenWidth, uint32_t screenHeight, uint32_t maxInstances = 50'000);

    void destroy() override;

    void submit(const Text &text);

    void begin();
    void end();

    const OrthographicCamera &camera() const { return *camera_; }
    uint32_t maxInstances() const { return maxInstances_; }

  private:
    void init();
    void initArrays();
    void initPipeline();
    void initBuffers();
    void initUniforms();

  private:
    struct VertexData {
      vec2 position;
      vec2 texCoords;
      Color color;
      Color outlineColor;
      vec2u texHandle;
      struct {
        float outlineWidth = 0.0f;
        float outlineEdge = 0.0f;
      } data;
    };

    struct CameraData {
      mat4 proj;
      mat4 view;
    };

  private:
    vec2u resolution_;
    std::unique_ptr<OrthographicCamera> camera_;

    uint32_t maxInstances_ = 1;

    uint32_t verticesCount_ = 0;
    uint32_t indicesCount_ = 0;

    uint32_t verticesSize_ = 0;
    uint32_t indicesSize_ = 0;
    uint32_t verticesBufferSize_ = 0;
    uint32_t indicesBufferSize_ = 0;

    VertexData *buffer_ = nullptr;
    std::unique_ptr<VertexData[]> bufferData_;
    std::unique_ptr<uint32_t[]> indices_;

    CameraData cameraData_;

    std::shared_ptr<gpu::Pipeline> pipeline_;
    std::shared_ptr<gpu::Buffer> vertexBuffer_;
    std::shared_ptr<gpu::Buffer> indexBuffer_;
    std::shared_ptr<gpu::Buffer> uniformBuffer_;
  };

}

#endif //XE_TEXT_RENDERER_HPP
