//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_WINDOW_HPP
#define XE_WINDOW_HPP

#include <xe/core/object.hpp>
#include <xe/core/timestep.hpp>

namespace xe {

  class XE_API Window : public Object {
  XE_OBJECT(Window, Object);
    friend class GPU;
    friend class Engine;
  public:
    struct Data;

    Window();
    ~Window() override;

    void forceExit();

    void setTitle(const string &title);
    void setIcon(uint width, uint height, byte *pixels);
    void setSwapInterval(bool enabled);

    bool isExisting() const;
    Timestep uptime() const;

    inline Params::Window params() const { return params_; }
    inline void setParams(const Params::Window &params) { params_ = params; }

  protected:
    void init();
    void pollEvents();
    void swap();
    void stop();

  protected:
    Data *data_;
    Params::Window params_{ };
  };

}

#endif //XE_WINDOW_HPP
