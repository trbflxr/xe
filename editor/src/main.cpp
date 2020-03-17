//
// Created by FLXR on 8/31/2019.
//

#include <xe/xe.hpp>
#include <xe/utils/logger.hpp>
#include <xe/graphics/renderer2d.hpp>
#include <xe/graphics/texture.hpp>

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

    layer_ = std::make_unique<TestLayer>();
    overlay_ = std::make_unique<TestOverlay>();

    texture_ = std::make_shared<Texture>();

    camera_ = std::make_unique<OrthographicCamera>(vec2(1280, 720), -10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
    camera_->setBackgroundColor(Color::Purple);

    renderer_ = std::make_unique<Renderer2d>(*camera_);
  }

  void onStart() override {
    overlay_->start();
    layer_->start();

    gpu::Texture::Info info;
    info.minFilter = TextureMinFilter::Linear;
    info.magFilter = TextureMagFilter::Linear;

    texture_->setInfo(info);
    texture_->loadFromFile("textures/test.png");
    texture_->setup();

  }

  void onStop() override {
    layer_->stop();
  }

  void onUpdate() override {
    overlay_->update(Engine::ref().delta());
    layer_->update(Engine::ref().delta());

    static float speed = 10.0f;

    if (Engine::isKeyPressed(Keyboard::S)) {
      camera_->transform().translateY(speed * Engine::ref().delta());
    } else if (Engine::isKeyPressed(Keyboard::W)) {
      camera_->transform().translateY(-speed * Engine::ref().delta());
    }

    if (Engine::isKeyPressed(Keyboard::D)) {
      camera_->transform().translateX(-speed * Engine::ref().delta());
    } else if (Engine::isKeyPressed(Keyboard::A)) {
      camera_->transform().translateX(speed * Engine::ref().delta());
    }

    camera_->update();
  }

  void onPreRender() override {
    camera_->updateUniforms();
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


    renderer_->begin();

    for (int32_t x = 0; x < 1280; x += 8) {
      for (int32_t y = 0; y < 720; y += 8) {
        renderer_->submit({x + 3.0f, y + 3.0f}, {6.0f, 6.0f}, Color::Olive, texture_);
      }
    }

    renderer_->end();
    renderer_->flush();

    layer_->render();
    overlay_->render();

    Engine::ref().composer().present();
  }

  void onKeyPressed(const Event::Key &key) override {
    overlay_->onKeyPressed(key);
    layer_->onKeyPressed(key);
  }


private:
  std::unique_ptr<TestLayer> layer_;
  std::unique_ptr<TestOverlay> overlay_;

  std::unique_ptr<OrthographicCamera> camera_;
  std::unique_ptr<Renderer2d> renderer_;

  std::shared_ptr<Texture> texture_;
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
