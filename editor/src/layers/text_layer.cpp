//
// Created by trbflxr on 3/23/2020.
//

#include "text_layer.hpp"

#include <xe/utils/logger.hpp>
#include <xe/ui/ui.hpp>

namespace xe {

  static const char *text_vert = R"(
    #version 460 core

    layout(location = 0) in vec2 a_position;
    layout(location = 1) in vec2 a_texCoords;

    out vec2 v_texCoords;

    layout(location = 0) uniform mat4 u_proj;
    layout(location = 1) uniform mat4 u_view;

    void main() {
      gl_Position = u_proj * u_view * vec4(a_position, 0.0, 1.0);
      v_texCoords = a_texCoords;
    }
  )";

  static const char *text_frag = R"(
    #version 460 core

    #extension GL_ARB_bindless_texture : require

    layout(location = 0) out vec4 color;

    in vec2 v_texCoords;

    uniform uvec2 u_sampler;

    const float smoothing = 1.0 / 64.0;
    const float offset = 0.45;

    float median(float r, float g, float b) {
      return max(min(r, g), min(max(r, g), b));
    }

    void main() {
      vec3 fontSample = texture(sampler2D(u_sampler), v_texCoords).rgb;
      float sigDist = median(fontSample.r, fontSample.g, fontSample.b);
      float opacity = smoothstep(offset - smoothing, offset + smoothing, sigDist);
      if(opacity < 0.05) {
        discard;
      }

      color = vec4(1.0, 0.0, 1.0, opacity);
    }
  )";

  uint16_t text_quad_indexData[] = {
      0, 2, 1, 0, 3, 2,
      4, 6, 5, 4, 7, 6,
      8, 10, 9, 8, 11, 10
  };

  void TextLayer::onStart() {
    camera_ = std::make_unique<OrthographicCamera>(vec2(1280, 720), 0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
    camera_->setBackgroundColor(Color::Purple);

    renderer_ = std::make_unique<Renderer2d>(*camera_);

    font_.loadFromFile("assets/fonts/consolata.otf", Charset::Ascii);

    texture_ = font_.atlas();

    uint32_t ch0 = 'W';
    uint32_t ch1 = 'a';
    uint32_t ch2 = 'o';

    Glyph g = font_.getGlyph(ch0);
    Glyph g1 = font_.getGlyph(ch1);
    Glyph g2 = font_.getGlyph(ch2);

    float k = font_.getKerning(ch0, ch1);
    float k1 = font_.getKerning(ch1, ch2);

    float fs = 256.0f;

    uint32_t vSize = 4 * 12;
    text_quad_vertexData_ = new float[vSize]{

        g.planeBounds.left * fs, g.planeBounds.bottom * fs, g.atlasBounds.left, g.atlasBounds.bottom,
        g.planeBounds.right * fs, g.planeBounds.bottom * fs, g.atlasBounds.right, g.atlasBounds.bottom,
        g.planeBounds.right * fs, g.planeBounds.top * fs, g.atlasBounds.right, g.atlasBounds.top,
        g.planeBounds.left * fs, g.planeBounds.top * fs, g.atlasBounds.left, g.atlasBounds.top,

        (g1.planeBounds.left + g.advance + k) * fs, g1.planeBounds.bottom * fs, g1.atlasBounds.left, g1.atlasBounds.bottom,
        (g1.planeBounds.right + g.advance + k) * fs, g1.planeBounds.bottom * fs, g1.atlasBounds.right, g1.atlasBounds.bottom,
        (g1.planeBounds.right + g.advance + k) * fs, g1.planeBounds.top * fs, g1.atlasBounds.right, g1.atlasBounds.top,
        (g1.planeBounds.left + g.advance + k) * fs, g1.planeBounds.top * fs, g1.atlasBounds.left, g1.atlasBounds.top,

        (g2.planeBounds.left + g.advance + k + g1.advance + k1) * fs, g2.planeBounds.bottom * fs, g2.atlasBounds.left, g2.atlasBounds.bottom,
        (g2.planeBounds.right + g.advance + k + g1.advance + k1) * fs, g2.planeBounds.bottom * fs, g2.atlasBounds.right, g2.atlasBounds.bottom,
        (g2.planeBounds.right + g.advance + k + g1.advance + k1) * fs, g2.planeBounds.top * fs, g2.atlasBounds.right, g2.atlasBounds.top,
        (g2.planeBounds.left + g.advance + k + g1.advance + k1) * fs, g2.planeBounds.top * fs, g2.atlasBounds.left, g2.atlasBounds.top,
    };

    //quad
    quad_.vertexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Vertex, Usage::Static, vSize * 4});
    quad_.indexBuff = Engine::ref().gpu().createBuffer(
        {BufferType::Index, Usage::Static, sizeof(text_quad_indexData)});

    gpu::Pipeline::Info::Shader quadShader;
    quadShader.vert = text_vert;
    quadShader.frag = text_frag;

    gpu::Pipeline::Info matInfo;
    matInfo.shader = quadShader;

    matInfo.attribs[0] = {"a_position", VertexFormat::Float2};
    matInfo.attribs[1] = {"a_texCoords", VertexFormat::Float2};

    matInfo.cull = Cull::Disabled;
    matInfo.blend.enabled = true;
    quad_.pipeline = Engine::ref().gpu().createPipeline(matInfo);

    DisplayList frame;
    frame.fillBufferCommand()
        .set_buffer(*quad_.vertexBuff)
        .set_data(text_quad_vertexData_)
        .set_size(vSize * 4);
    frame.fillBufferCommand()
        .set_buffer(*quad_.indexBuff)
        .set_data(text_quad_indexData)
        .set_size(sizeof(text_quad_indexData));
    Engine::ref().executeOnGpu(std::move(frame));
  }

  void TextLayer::onStop() {
    font_->destroy();
    camera_->destroy();
    texture_->destroy();
    renderer_->destroy();

    Engine::ref().gpu().destroyResource(*quad_.pipeline);
    Engine::ref().gpu().destroyResource(*quad_.indexBuff);
    Engine::ref().gpu().destroyResource(*quad_.vertexBuff);
  }

  void TextLayer::onPreRender() {
    camera_->updateUniforms();
  }

  void TextLayer::onRender() {
    renderer_->begin();

    renderer_->submit({0.0f, 0.0f}, texture_->size(), Color::White, texture_);

    renderer_->end();

    vec2u sampler;
    uint64_t handle = texture_->handle();
    std::memcpy(&sampler, &handle, sizeof(vec2u));

    DisplayList frame;
    frame.setupViewCommand()
        .set_viewport(camera_->viewport())
        .set_framebuffer(Engine::ref().composer().framebuffer())
        .set_colorAttachment(0, true);
    frame.clearCommand()
        .set_color(Color::Clear)
        .set_clearColor(false)
        .set_clearDepth(true);
    frame.setupPipelineCommand()
        .set_pipeline(*quad_.pipeline)
        .set_buffer(0, *quad_.vertexBuff)
        .set_uniform(0, {"u_proj", &camera_->projection(), sizeof(mat4)})
        .set_uniform(1, {"u_view", &camera_->view(), sizeof(mat4)})
        .set_uniform(2, {"u_sampler", &sampler, sizeof(vec2u)});
    frame.renderCommand()
        .set_indexBuffer(*quad_.indexBuff)
        .set_count(sizeof(text_quad_indexData) / sizeof(uint16_t))
        .set_type(IndexFormat::Uint16);
    Engine::ref().executeOnGpu(std::move(frame));
  }

  void TextLayer::onUpdate() {
    static float speed = 10.0f;

    auto ts = Engine::ref().delta();

    if (Engine::isKeyPressed(Keyboard::S)) {
      camera_->transform().translateY(speed * ts);
    } else if (Engine::isKeyPressed(Keyboard::W)) {
      camera_->transform().translateY(-speed * ts);
    }

    if (Engine::isKeyPressed(Keyboard::D)) {
      camera_->transform().translateX(-speed * ts);
    } else if (Engine::isKeyPressed(Keyboard::A)) {
      camera_->transform().translateX(speed * ts);
    }
  }

  bool TextLayer::onKeyPressed(Event::Key e) {
    return false;
  }

  bool TextLayer::onTextEntered(Event::Text text) {
//    XE_CORE_CRITICAL("[TextLayer] unicode: {}", text.unicode);
    return false;
  }

  bool TextLayer::onUi() {
    ImGui::Text("TextLayer:");
    return false;
  }

}

