//
// Created by FLXR on 8/31/2019.
//

#include <xe/core/application.hpp>
#include <xe/utils/debug_trace.hpp>
#include <xe/utils/logger.hpp>

using namespace xe;

class Editor : public Application {
XE_OBJECT(Editor, Application);
public:
  Editor() {
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

  void renderUpdate() override {
    DrawList dl;
    dl.clearCommand()
        .set_color({1, 0, 1, 1});
    Engine::ref().submitDrawList(std::move(dl));
  }
};

XE_DEFINE_ENTRY_POINT(Editor);
