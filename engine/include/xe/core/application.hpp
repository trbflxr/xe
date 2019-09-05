//
// Created by FLXR on 9/4/2019.
//

#ifndef XE_APPLICATION_HPP
#define XE_APPLICATION_HPP

#include <xe/core/engine.hpp>

namespace xe {

  class XE_API Application : public Object {
  XE_OBJECT(Application, Object);
  public:
    explicit Application();
    ~Application() override;

    bool isExisting() const;

    inline void setTimestep(float step) { framerate_.timeStep = step; }
    inline void setMaxSteps(uint steps) { framerate_.maxSteps = steps; }

    inline Timestep delta() const { return framerate_.timeStep; }
    inline uint fps() const { return framerate_.fps; }

    int32 run(int32 argc, char **argv);

  protected:
    virtual void init() { }
    virtual void start() { }
    virtual void preUpdate() { }
    virtual void update(Timestep ts) { }
    virtual void postUpdate() { }
    virtual void renderPreUpdate() { }
    virtual void renderUpdate() { }
    virtual void renderPostUpdate() { }
    virtual void stop() { }

  private:
    void initInternal();
    void startInternal();
    void preUpdateInternal();
    void updateInternal(Timestep ts);
    void postUpdateInternal();
    void renderPreUpdateInternal();
    void renderUpdateInternal();
    void renderPostUpdateInternal();
    void stopInternal();

  private:
    bool exit_;

    struct Framerate {
      float timeStep;
      uint maxSteps;
      uint fps;
    } framerate_;
  };

}

#define XE_DEFINE_ENTRY_POINT(className) \
int32 main(int32 argc, char** argv) { \
  className app; \
  return app.run(argc, argv); \
}

#endif //XE_APPLICATION_HPP
