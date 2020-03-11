//
// Created by FLXR on 9/11/2019.
//

#include "test_layer.hpp"
#include <xe/core/vfs.hpp>
#include <xe/utils/logger.hpp>
#include <xe/ui/ui.hpp>

using namespace xe;

namespace cube {
  float vertexData[] = {
      -1.0, -1.0, 1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      1.0, -1.0, 1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      1.0, 1.0, 1.0, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      -1.0, 1.0, 1.0, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

      -1.0, -1.0, -1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      1.0, -1.0, -1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      1.0, 1.0, -1.0, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      -1.0, 1.0, -1.0, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
  };

  uint16_t indexData[] = {
      0, 1, 2,
      2, 3, 0,

      1, 5, 6,
      6, 2, 1,

      7, 6, 5,
      5, 4, 7,

      4, 0, 3,
      3, 7, 4,

      4, 5, 1,
      1, 0, 4,

      3, 2, 6,
      6, 7, 3
  };
}

namespace quad {
  float vertexData[] = {
      -1.0, -1.0, 0.0, 0.0, 0.0,
      1.0, -1.0, 0.0, 1.0, 0.0,
      1.0, 1.0, 0.0, 1.0, 1.0,
      -1.0, 1.0, 0.0, 0.0, 1.0
  };
  uint16_t indexData[] = {0, 2, 1, 0, 3, 2};
}

TestLayer::TestLayer() :
    texData_(nullptr),
    instances_(static_cast<int32_t>(State::INSTANCES / 8.0f)) { }

void TestLayer::start() {
  Engine::ref().setUiFunction(TestLayer::uiFunc, this);

  std::shared_ptr<Scene> scene = std::make_shared<Scene>();
  Engine::ref().loadScene(scene);

  camera_ = std::make_shared<GameObject>();
  camera_->setName("Camera");
  camera_->transform()->setLocalPositionX(2.0f);

  state_.cube.proj = mat4::perspective(60.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
  state_.cube.view = mat4::transformation({-10, 25, 60},
                                          quat(vec3::unitY(), 90.0f) * quat(vec3::unitX(), 30.0f)).inverse();


  state_.quad.proj = mat4::perspective(60.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
  state_.quad.view = mat4::translation({0.0f, 0.0f, 2.0f}).inverse();


  state_.cube.vertexBuff = Engine::ref().gpu().createBuffer(
      {BufferType::Vertex, Usage::Static, sizeof(cube::vertexData)});
  state_.cube.indexBuff = Engine::ref().gpu().createBuffer(
      {BufferType::Index, Usage::Static, sizeof(cube::indexData)});
  state_.cube.instanceBuffer = Engine::ref().gpu().createBuffer(
      {BufferType::Vertex, Usage::Dynamic, sizeof(state_.cube.instancePositions)});

  state_.quad.vertexBuff = Engine::ref().gpu().createBuffer(
      {BufferType::Vertex, Usage::Static, sizeof(quad::vertexData)});
  state_.quad.indexBuff = Engine::ref().gpu().createBuffer(
      {BufferType::Index, Usage::Static, sizeof(quad::indexData)});

  {
    gpu::Pipeline::Info matInfo;
    matInfo.shader = *Engine::ref().assetManager().getShader("test");

    matInfo.attribs[0] = {"a_position", VertexFormat::Float3};
    matInfo.attribs[1] = {"a_texCoords", VertexFormat::Float2};
    matInfo.attribs[2] = {"a_color", VertexFormat::Float4};
    matInfo.attribs[3] = {"a_instancePosition", VertexFormat::Float4, 1, VertexStep::PerInstance};

    matInfo.textures[0] = TextureType::T2D;

    gpu::Texture::Info texInfo;
    texData_ = gpu::Texture::loadFromFile("textures/test.png", texInfo);
    texInfo.magFilter = TextureMagFilter::Linear;
    texInfo.minFilter = TextureMinFilter::LinearMipmapLinear;
    state_.cube.texture = Engine::ref().gpu().createTexture(texInfo);

    state_.cube.material = Engine::ref().gpu().createPipeline(matInfo);
    {
      DisplayList frame;
      frame.fillBufferCommand()
          .set_buffer(state_.cube.vertexBuff)
          .set_data(cube::vertexData)
          .set_size(sizeof(cube::vertexData));
      frame.fillBufferCommand()
          .set_buffer(state_.cube.indexBuff)
          .set_data(cube::indexData)
          .set_size(sizeof(cube::indexData));
      frame.fillTextureCommand()
          .set_texture(state_.cube.texture)
          .set_data0(texData_)
          .set_buildMipmap(true);
      Engine::ref().submitDrawList(std::move(frame));
    }
  }

  {
    gpu::Pipeline::Info matInfo;
    matInfo.shader = *Engine::ref().assetManager().getShader("fb_test");

    matInfo.attribs[0] = {"a_position", VertexFormat::Float3};
    matInfo.attribs[1] = {"a_texCoords", VertexFormat::Float2};

    matInfo.textures[0] = TextureType::T2D;
    matInfo.textures[1] = TextureType::T2D;
    matInfo.cull = Cull::Disabled;
    state_.quad.material = Engine::ref().gpu().createPipeline(matInfo);

    {
      DisplayList frame;
      frame.fillBufferCommand()
          .set_buffer(state_.quad.vertexBuff)
          .set_data(quad::vertexData)
          .set_size(sizeof(quad::vertexData));
      frame.fillBufferCommand()
          .set_buffer(state_.quad.indexBuff)
          .set_data(quad::indexData)
          .set_size(sizeof(quad::indexData));
      Engine::ref().submitDrawList(std::move(frame));
    }

    gpu::Texture::Info fbColor0 = {800, 600};
    fbColor0.format = TexelsFormat::Rgba8;

    gpu::Texture::Info fbColor1 = {800, 600};
    fbColor1.format = TexelsFormat::Rgba8;

    gpu::Texture::Info fbDepth = {800, 600};
    fbDepth.format = TexelsFormat::Depth16;

    gpu::Framebuffer::Info fbInfo;
    fbInfo.colorAttachmentInfo[0] = fbColor0;
    fbInfo.colorAttachmentInfo[1] = fbColor1;
    fbInfo.depthStencilAttachmentInfo = fbDepth;
    fbInfo.colorAttachmentsSize = 2;

    state_.fb = Engine::ref().gpu().createFramebuffer(fbInfo);
  }
}

TestLayer::~TestLayer() {

}

void TestLayer::stop() {

}

void TestLayer::render() {
  DisplayList frame;
  frame.setupViewCommand()
      .set_viewport({0, 0, 800, 600})
      .set_framebuffer(state_.fb)
      .set_colorAttachment(0, true)
      .set_colorAttachment(1, true);
  frame.clearCommand()
      .set_color(Color(0xF4A1AA))
      .set_clearColor(true)
      .set_clearDepth(true);
  frame.fillBufferCommand()
      .set_buffer(state_.cube.instanceBuffer)
      .set_data(state_.cube.instancePositions)
      .set_size(sizeof(state_.cube.instancePositions));
  frame.setupPipelineCommand()
      .set_pipeline(state_.cube.material)
      .set_buffer(0, state_.cube.vertexBuff)
      .set_buffer(1, state_.cube.instanceBuffer)
      .set_uniform(0, {"u_model", &state_.cube.model, sizeof(mat4)})
      .set_uniform(1, {"u_view", &state_.cube.view, sizeof(mat4)})
      .set_uniform(2, {"u_projection", &state_.cube.proj, sizeof(mat4)})
      .set_texture(0, state_.cube.texture);
  frame.renderCommand()
      .set_indexBuffer(state_.cube.indexBuff)
      .set_count(sizeof(cube::indexData) / sizeof(uint16_t))
      .set_type(IndexFormat::Uint16)
      .set_instances(instances_);

  Color tint = Color::Aqua;

  //framebuffer
  frame.setupViewCommand()
      .set_viewport({0, 0, 800, 600});
  frame.clearCommand()
      .set_color(Color(0x131313))
      .set_clearColor(true)
      .set_clearDepth(true);
  frame.setupPipelineCommand()
      .set_pipeline(state_.quad.material)
      .set_buffer(0, state_.quad.vertexBuff)
      .set_uniform(0, {"u_model", &state_.quad.model, sizeof(mat4)})
      .set_uniform(1, {"u_view", &state_.quad.view, sizeof(mat4)})
      .set_uniform(2, {"u_projection", &state_.quad.proj, sizeof(mat4)})
      .set_uniform(3, {"u_tint", &tint, sizeof(Color)})
      .set_texture(0, state_.fb.colorAttachment(0))
      .set_texture(1, state_.fb.colorAttachment(1));
  frame.renderCommand()
      .set_indexBuffer(state_.quad.indexBuff)
      .set_count(sizeof(quad::indexData) / sizeof(uint16_t))
      .set_type(IndexFormat::Uint16);

  Engine::ref().submitDrawList(std::move(frame));
}

void TestLayer::update(Timestep ts) {
  static float v = 0;
  for (size_t i = 0; i < (size_t) instances_; ++i) {
    state_.cube.instancePositions[i] = {
        (i / 100) * 2.0f,
        750.0f * (float) (sin(i * XE_MATH_PI / 10.0f + v) * ts),
        (float) (i % 100) * 1.2f,
        0.0f
    };
  }
  v += 0.01f;

  static float angle = 0.0f;
  angle += 45.0f * ts;

  state_.quad.model = mat4::transformation(vec3(), {{0, 1, 0}, angle});

  camera_->transform()->rotate({0, ts, 0});

//  XE_CORE_CRITICAL(camera_->transform()->localRotation().toEulerAngles());
}

bool TestLayer::onKeyPressed(const Event::Key &e) {
  XE_INFO("[TestLayer] key pressed ({})", e.code);
  return false;
}

void TestLayer::uiFunc(void *data) {
  TestLayer *tl = (TestLayer *) data;

  static const uint32_t flags = ImGuiWindowFlags_NoDocking |
                                ImGuiWindowFlags_MenuBar |
                                ImGuiWindowFlags_NoTitleBar |
                                ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_AlwaysAutoResize |
                                ImGuiWindowFlags_NoSavedSettings |
                                ImGuiWindowFlags_NoFocusOnAppearing |
                                ImGuiWindowFlags_NoNav;


  const float DISTANCE = 10.0f;
  static int32_t corner = 0;

  static bool showDemo = false;

  ImGuiIO &io = ImGui::GetIO();

  if (corner != -1) {
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    const ImVec2 windowPos = ImVec2(
        (corner & 1) ? (viewport->Pos.x + viewport->Size.x - DISTANCE) : (viewport->Pos.x + DISTANCE),
        (corner & 2) ? (viewport->Pos.y + viewport->Size.y - DISTANCE) : (viewport->Pos.y + DISTANCE));

    const ImVec2 windowPosPivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPosPivot);
    ImGui::SetNextWindowViewport(viewport->ID);
  }

  ImGui::SetNextWindowBgAlpha(0.7f);
  if (ImGui::Begin("MainMenu", nullptr, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | flags)) {
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Examples")) {
        if (ImGui::MenuItem("Demo", nullptr)) {
          showDemo = !showDemo;
        }
        ImGui::EndMenu();
      }

      ImGui::EndMenuBar();
    }
  }

  ImGui::Text("uptime: %.2f", Engine::ref().window().uptime().seconds());
  ImGui::Separator();
  ImGui::Dummy({10.0f, 0.0f});

  char buff[2048];
  memset(buff, 0, 2048);
  ImGui::SliderInt("Instances", &tl->instances_, 1, State::INSTANCES);
  ImGui::Dummy({10.0f, 0.0f});

  static const uint32_t TIMES_SIZE = 90;
  static float times[TIMES_SIZE] = {0};
  static uint32_t index = 0;

  index = index >= TIMES_SIZE ? 0 : index;
  times[index++] = io.DeltaTime * 1000.0f;

  ImGui::PlotLines("", times, TIMES_SIZE, index, "Frametime (ms)", 0.0f, 100.0f, ImVec2(0, 80));
  ImGui::Text("Frametime: %.3fms", io.DeltaTime * 1000.0f);

  ImGui::Text("Right-click to change position");

  if (ImGui::BeginPopupContextWindow()) {
    if (ImGui::MenuItem("Custom", nullptr, corner == -1)) corner = -1;
    if (ImGui::MenuItem("Top-left", nullptr, corner == 0)) corner = 0;
    if (ImGui::MenuItem("Top-right", nullptr, corner == 1)) corner = 1;
    if (ImGui::MenuItem("Bottom-left", nullptr, corner == 2)) corner = 2;
    if (ImGui::MenuItem("Bottom-right", nullptr, corner == 3)) corner = 3;
    ImGui::EndPopup();
  }
  ImGui::End();

  if (showDemo) {
    ImGui::ShowDemoWindow(&showDemo);
  }
}
