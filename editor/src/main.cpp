//
// Created by FLXR on 8/31/2019.
//

#include <xe/core/application.hpp>
#include <xe/utils/logger.hpp>
#include <xe/core/vfs.hpp>
#include "layers/test_layer.hpp"
#include "layers/test_overlay.hpp"

using namespace xe;

class Editor : public Application {
XE_OBJECT(Editor, Application);
public:
  Editor() {
    xe::Logger::setLogLevel(LogLevel::Info, LogLevel::Info);

    Params params = {{800, 600,
                                "test / жопа",
                                    true, true,
                         0, 0, nullptr},
                     {128, 128, 64, 128}};

    Engine::ref().setParams(params);
  }

protected:
  void init() override {
    VFS::mount("shaders", "assets/shaders/");
    VFS::mount("textures", "assets/textures/");

    ref_ptr<TestLayer> l = make_ref<TestLayer>(*this);
    ref_ptr<TestOverlay> o = make_ref<TestOverlay>(*this);
    pushLayer(l);
    pushOverlay(o);
  }

};

int32 main(int32 argc, char **argv) {
  Editor app;
  return app.run(argc, argv);
}
