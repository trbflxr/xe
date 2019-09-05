//
// Created by FLXR on 8/31/2019.
//

#include <xe/core/application.hpp>
#include <xe/utils/debug_trace.hpp>
#include <xe/utils/logger.hpp>

using namespace xe;

static float vertexData[] = {
    -0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 0.0,
    0.5, -0.5, 0.0, 1.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 1.0,
    -0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0
};

static uint16 indexData[] = {
    0, 1, 2, 2, 3, 0
};

class Editor : public Application {
XE_OBJECT(Editor, Application);
public:
  Editor() {
    xe::Logger::setLogLevel(LogLevel::Info, LogLevel::Info);

    Params params = {{800, 600,
                                "test",
                                     true,
                         0, 0, nullptr},
                     {128, 128, 128, 128}};

    Engine::ref().setParams(params);


  }

protected:
  void init() override {

  }

  void start() override {
    vbo_ = Engine::ref().gpu().createBuffer({BufferType::Vertex, Usage::Static, sizeof(vertexData)});
    ibo_ = Engine::ref().gpu().createBuffer({BufferType::Index, Usage::Static, sizeof(indexData)});

    gpu::Material::Info matInfo;
    matInfo.shader.vert = R"(
        #version 330 core
        in vec2 a_position;
        in vec3 a_color;
        in vec2 a_texCoords;
        out vec3 v_color;
        out vec2 v_texCoords;
        void main() {
          gl_Position = vec4(a_position, 0.0, 1.0);
          v_color = a_color;
          v_texCoords = a_texCoords;
        })";
    matInfo.shader.frag = R"(
        #version 330 core
        out vec4 color;
        in vec3 v_color;
        in vec2 v_texCoords;
        uniform sampler2D u_tex2d0;
        void main() {
          //color = vec4(v_color, 1.0);
          color = vec4(v_color * texture(u_tex2d0, v_texCoords).rgb, 1.0);
          color = texture(u_tex2d0, v_texCoords);
        })";

    matInfo.attribs[0] = {"a_position", VertexFormat::Float2};
    matInfo.attribs[1] = {"a_color", VertexFormat::Float3};
    matInfo.attribs[2] = {"a_texCoords", VertexFormat::Float2};

    matInfo.textures[0] = TextureType::T2D;


    gpu::Texture::Info texInfo;
    texData_ = gpu::Texture::loadFromFile("assets/textures/test.png", texInfo);
    texInfo.magFilter = TextureMagFilter::Linear;
    texInfo.minFilter = TextureMinFilter::LinearMipmapLinear;
    texture_ = Engine::ref().gpu().createTexture(texInfo);

    material_ = Engine::ref().gpu().createMaterial(matInfo);

    DrawList frame;
    frame.fillBufferCommand()
        .set_buffer(vbo_)
        .set_data(vertexData)
        .set_size(sizeof(vertexData));
    frame.fillBufferCommand()
        .set_buffer(ibo_)
        .set_data(indexData)
        .set_size(sizeof(indexData));
    frame.fillTextureCommand()
        .set_texture(texture_)
        .set_data0(texData_)
        .set_buildMipmap(true);
    Engine::ref().submitDrawList(std::move(frame));
  }

  void renderUpdate() override {
    DrawList frame;
    frame.setupViewCommand()
        .set_viewport({0, 0, 800, 600});
    frame.clearCommand()
        .set_color(Color(0xFFA6A6A6));
    frame.setupMaterialCommand()
        .set_material(material_)
        .set_buffer(0, vbo_)
        .set_texture(0, texture_);
    frame.renderCommand()
        .set_indexBuffer(ibo_)
        .set_count(sizeof(indexData) / sizeof(uint16))
        .set_type(IndexFormat::Uint16);
    Engine::ref().submitDrawList(std::move(frame));
  }

private:
  gpu::Buffer vbo_;
  gpu::Buffer ibo_;
  gpu::Material material_;
  gpu::Texture texture_;
  void *texData_;
};

XE_DEFINE_ENTRY_POINT(Editor);
