//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_WINDOW_HPP
#define XE_WINDOW_HPP

#include <xe/core/object.hpp>

namespace xe {

  namespace detail {
    struct WindowData;
  }

  class XE_API Window : public Object {
  XE_OBJECT(Window, Object);
    friend class RenderDevice;
    friend class Engine;
  public:
    Window();
    ~Window() override;

    void forceExit();

    void setTitle(const string &title);
    void setIcon(uint width, uint height, byte *pixels);
    void setSwapInterval(bool enabled);

    bool shouldClose() const;
    double uptime() const;

    inline Params::Window params() const { return params_; }

  protected:
    void init(const Params::Window &params);
    void pollEvents();
    void swap();
    void stop();

  protected:
    detail::WindowData *data_;
    Params::Window params_{ };
  };

}

#endif //XE_WINDOW_HPP
