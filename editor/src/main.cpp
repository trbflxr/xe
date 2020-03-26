//
// Created by FLXR on 8/31/2019.
//

#include <xe/xe.hpp>
#include <xe/utils/logger.hpp>
#include <xe/ui/imgui/imgui.h>

#include "layers/test_layer.hpp"
#include "layers/test_overlay.hpp"
#include "layers/renderer2d_layer.hpp"
#include "layers/text_layer.hpp"

namespace xe {

  class Editor : public Application {
  XE_OBJECT(Editor, Application);
  public:
    Editor() {
      xe::Logger::setLogLevel(LogLevel::Trace, LogLevel::Trace);

      Engine::ref().setUiFunction(Editor::uiFunc, this);
    }

  protected:
    void onInit() override {
      Engine::ref().vfs().mount(".");
      Engine::ref().vfs().mount("assets");

      layers_.emplace_back(std::make_shared<Renderer2dLayer>());
      layers_.emplace_back(std::make_shared<TestLayer>());
      layers_.emplace_back(std::make_shared<TestOverlay>());
      layers_.emplace_back(std::make_shared<TextLayer>());
    }

    void onStart() override {
      for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
        (*l)->onStart();
      }
    }

    void onStop() override {
      for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
        (*l)->onStop();
      }
    }

    void onUpdate() override {
      for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
        (*l)->onUpdate();
      }
    }

    void onPreRender() override {
      for (auto &&l : layers_) {
        l->onPreRender();
      }
    }

    void onRender() override {
      // clear
      DisplayList frame;
      frame.setupViewCommand()
          .set_viewport({0, 0, 1280, 720})
          .set_framebuffer(Engine::ref().composer().framebuffer())
          .set_colorAttachment(0, true);
      frame.clearCommand()
          .set_color(Color::Teal)
          .set_clearColor(true)
          .set_clearDepth(true);
      Engine::ref().executeOnGpu(std::move(frame));

      for (auto &&l : layers_) {
        l->onRender();
      }

      Engine::ref().composer().present();
    }

    void onKeyPressed(const Event::Key key) override {
      for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
        (*l)->onKeyPressed(key);
      }
    }

    void onTextEntered(Event::Text text) override {
      for (auto &&l = layers_.rbegin(); l != layers_.rend(); ++l) {
        (*l)->onTextEntered(text);
      }
    }

    static void uiFunc(void *data) {
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
      ImGui::Text("fps: %u / ups: %u", Engine::ref().fps(), Engine::ref().ups());
      ImGui::Text("draw calls: %u", GPU::drawCalls());
      ImGui::Text("gpu commands: %u", GPU::gpuCommands());
      ImGui::Text("used textures: %u / %u", Engine::ref().gpu().usedTextures(), Engine::ref().gpu().maxTextures());
      ImGui::Text("used buffers: %u / %u", Engine::ref().gpu().usedBuffers(), Engine::ref().gpu().maxBuffers());
      ImGui::Text("used pipelines: %u / %u", Engine::ref().gpu().usedPipelines(), Engine::ref().gpu().maxPipelines());
      ImGui::Text("used framebuffers: %u / %u", Engine::ref().gpu().usedFramebuffers(), Engine::ref().gpu().maxFramebuffers());

      //layers ui
      auto &&editor = static_cast<Editor *>(data);
      for (auto &&l = editor->layers_.rbegin(); l != editor->layers_.rend(); ++l) {
        ImGui::Separator();
        ImGui::Dummy({10.0f, 0.0f});
        (*l)->onUi();
      }

      ImGui::Separator();
      ImGui::Dummy({10.0f, 0.0f});

      static const uint32_t TIMES_SIZE = 90;
      static float times[TIMES_SIZE] = {0};
      static uint32_t index = 0;

      index = index >= TIMES_SIZE ? 0 : index;
      times[index++] = io.DeltaTime * 1000.0f;

      ImGui::PlotLines("", times, TIMES_SIZE, index, "Frametime (ms)", 0.0f, 100.0f, ImVec2(0, 80));
      ImGui::Text("Frametime: %.3fms", io.DeltaTime * 1000.0f);
      ImGui::Text("Deltatime: %.3fms", Engine::ref().delta().mills());

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

  private:
    std::vector<std::shared_ptr<LayerBase>> layers_;
  };

}

int32_t main(int32_t argc, char **argv) {
  static xe::Params defaultParams = {{1280, 720,
                                                 "test / жопа",
                                                     true, true,
                                         0, 0, nullptr},
                                     {128,  128, 64, 128}};

  //step #0 create engine
  xe::Engine engine(argc, argv);

  xe::Params params;

  //load config
  xe::File paramsFile("config.json");
  if (paramsFile.load()) {
    *paramsFile.getNode() >> params;
  } else {
    params = defaultParams;
  }

  engine.init(params);
  engine.setApp(std::make_shared<xe::Editor>());

  int32_t exitCode = engine.run();

  //save config
  *paramsFile.getNode() << engine.getParams();
  paramsFile.write(xe::Node::Format::Beautified);

  return exitCode;
}
