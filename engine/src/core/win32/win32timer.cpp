//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/timer.hpp>

namespace xe {

  struct TimerMembers {
    LARGE_INTEGER start;
    double frequency;
  };


  Timer::Timer() :
      _members(new(_reserved) TimerMembers()) {

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    _members->frequency = 1.0f / frequency.QuadPart;

    reset();
  }

  void Timer::reset() {
    QueryPerformanceCounter(&_members->start);
  }

  Timestep Timer::elapsed() {
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);

    LONGLONG cycles = current.QuadPart - _members->start.QuadPart;
    return static_cast<float>(cycles * _members->frequency);
  }

}
