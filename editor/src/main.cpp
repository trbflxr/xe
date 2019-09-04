//
// Created by FLXR on 8/31/2019.
//

#include <xe/core/engine.hpp>
#include <xe/utils/debug_trace.hpp>
#include <xe/utils/logger.hpp>

using namespace xe;

namespace xe {
  class Application {
  public:
    Application() {
      Params params = {{800, 600,
                                  "test",
                                       true,
                           0, 0, nullptr},
                       {128, 128, 128, 128}};

      Engine::ref().setParams(params);


    }

    int32 run() {
      XE_TRACE_INIT("xetrace.json");
      XE_TRACE_META_THREAD_NAME("Update thread");

      XE_TRACE_BEGIN("XE", "Application init");
      XE_CORE_INFO("[ENGINE] Starting application");
      Engine::ref().init();
      XE_TRACE_END("XE", "Application init");

      XE_TRACE_BEGIN("XE", "Application start");
      Engine::ref().start();
      XE_TRACE_END("XE", "Application start");

      while (!Engine::ref().isExisting()) {
        XE_TRACE_BEGIN("XE", "Frame");
        XE_CORE_TRACE("[ENGINE] New render frame");

        XE_TRACE_BEGIN("XE", "Loop update");
        Engine::ref().preUpdate();
        XE_TRACE_BEGIN("XE", "Application update");
        Engine::ref().update(0.0f);
        XE_TRACE_END("XE", "Application update");
        Engine::ref().postUpdate();
        XE_TRACE_END("XE", "Loop update");

        XE_TRACE_BEGIN("XE", "Render update");
        Engine::ref().renderPreUpdate();

        DrawList dl;
        dl.clearCommand()
            .set_color({1, 0, 1, 1});
        Engine::ref().submitDrawList(std::move(dl));

        Engine::ref().renderUpdate();
        Engine::ref().renderPostUpdate();
        XE_TRACE_END("XE", "Render update");

        XE_TRACE_END("XE", "Frame");
      }
      XE_CORE_INFO("[ENGINE] Stopping application");
      Engine::ref().stop();

      XE_TRACE_SHUTDOWN();

      XE_CORE_INFO("[ENGINE] Application exit code 0");
      return 0;
    }
  };
}

int main() {

  Application app;
  app.run();

}
