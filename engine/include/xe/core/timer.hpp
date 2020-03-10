//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_TIMER_HPP
#define XE_TIMER_HPP

#include <xe/common.hpp>
#include <xe/core/object.hpp>
#include <xe/core/timestep.hpp>

namespace xe {

  struct TimerMembers;

  class XE_API Timer : public Object {
  XE_OBJECT(Timer, Object);
  public:
    Timer();

    void reset();

    Timestep elapsed();

  private:
    uint8_t _reserved[32];
    TimerMembers *_members;
  };

}

#endif //XE_TIMER_HPP
