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
  void init() override {
    Engine::ref().vfs().mount(".");
    Engine::ref().vfs().mount("assets");

    std::shared_ptr<TestLayer> l = std::make_shared<TestLayer>(*this);
    std::shared_ptr<TestOverlay> o = std::make_shared<TestOverlay>(*this);
    pushLayer(l);
    pushOverlay(o);
  }

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

  Editor app;
  int32_t exitCode = app.run();

  //save config
  *paramsFile.getNode() << engine.getParams();
  paramsFile.write(Node::Format::Beautified);

  return exitCode;
}
