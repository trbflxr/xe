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

    layer_ = std::make_unique<TestLayer>();
    overlay_ = std::make_unique<TestOverlay>();
  }

  void onStart() override {
    overlay_->start();
    layer_->start();
  }

  void onStop() override {
    layer_->stop();
  }

  void onUpdate() override {
    overlay_->update(Engine::ref().delta());
    layer_->update(Engine::ref().delta());
  }

  void onRender() override {
    layer_->render();
    overlay_->render();
  }

  void onKeyPressed(const Event::Key &key) override {
    overlay_->onKeyPressed(key);
    layer_->onKeyPressed(key);
  }


private:
  std::unique_ptr<TestLayer> layer_;
  std::unique_ptr<TestOverlay> overlay_;
};

int32_t main(int32_t argc, char **argv) {
  static Params defaultParams = {{800, 600,
                                            "test / жопа",
                                                true, true,
                                     0, 0, nullptr},
                                 {128, 128, 64, 128}};

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

  engine.setParams(params);
  engine.setApp(std::make_shared<Editor>());

  int32_t exitCode = engine.run();

  //save config
  *paramsFile.getNode() << engine.getParams();
  paramsFile.write(Node::Format::Beautified);

  return exitCode;
}
