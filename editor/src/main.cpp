//
// Created by FLXR on 8/31/2019.
//

#include <xe/xe.hpp>
#include <xe/utils/logger.hpp>

#include "layers/test_layer.hpp"
#include "layers/test_overlay.hpp"

using namespace xe;

class Editor : public Application {
XE_OBJECT(Editor, Application);
public:
  Editor() {
    xe::Logger::setLogLevel(LogLevel::Info, LogLevel::Info);
  }

protected:
  void onInit() override {
    Engine::ref().vfs().mount(".");
    Engine::ref().vfs().mount("assets");

    vec2u size = {Engine::ref().getParams().window.width, Engine::ref().getParams().window.height};

    camera_ = std::make_shared<Camera>(size);
    camera_->init();

    camera_->set_fov(70.0f);
    camera_->set_aspect(static_cast<float>(size.x) / size.y);
    camera_->set_nearPlane(1.0f);
    camera_->set_farPlane(1000.0f);
    camera_->set_backgroundColor(Color::Clear);
    camera_->set_clearColor(false);
    camera_->set_clearDepth(false);

    camera_->transform().translateZ(-4.0f);

    layer_ = std::make_unique<TestLayer>(camera_.get());
    overlay_ = std::make_unique<TestOverlay>(camera_.get());
  }

  void onStart() override {
    overlay_->start();
    layer_->start();
  }

  void onStop() override {
    layer_->stop();
  }

  void onUpdate() override {
    camera_->update();

    overlay_->update(Engine::ref().delta());
    layer_->update(Engine::ref().delta());
  }

  void onPreRender() override {
    camera_->onPreRender();
  }

  void onRender() override {
    // clear
    DisplayList frame;
    frame.setupViewCommand()
        .set_viewport({0, 0, camera_->viewport().x, camera_->viewport().y})
        .set_framebuffer(Engine::ref().composer().framebuffer())
        .set_colorAttachment(0, true);
    frame.clearCommand()
        .set_color(Color::Teal)
        .set_clearColor(true)
        .set_clearDepth(true);
    Engine::ref().submitDrawList(std::move(frame));

    layer_->render();
    overlay_->render();

    Engine::ref().composer().present();
  }

  void onKeyPressed(const Event::Key &key) override {
    overlay_->onKeyPressed(key);
    layer_->onKeyPressed(key);
  }


private:
  std::shared_ptr<Camera> camera_;

  std::unique_ptr<TestLayer> layer_;
  std::unique_ptr<TestOverlay> overlay_;
};

int32_t main(int32_t argc, char **argv) {
  static Params defaultParams = {{1280, 720,
                                             "test / жопа",
                                                 true, true,
                                     0, 0, nullptr},
                                 {128,  128, 64, 128}};

  //step #0 create engine
  Engine engine(argc, argv);

  Params params;

  //load config
  File paramsFile("config.json");
  if (paramsFile.load()) {
    *paramsFile.getNode() >> params;
  } else {
    params = defaultParams;
  }

  engine.init(params);
  engine.setApp(std::make_shared<Editor>());

  int32_t exitCode = engine.run();

  //save config
  *paramsFile.getNode() << engine.getParams();
  paramsFile.write(Node::Format::Beautified);

  return exitCode;
}
