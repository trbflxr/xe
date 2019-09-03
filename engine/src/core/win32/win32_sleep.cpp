//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/sleep.hpp>
#include <sapi.h>

namespace xe {

  void sleep(uint time) {
    TIMECAPS tc;
    timeGetDevCaps(&tc, sizeof(TIMECAPS));

    timeBeginPeriod(tc.wPeriodMin);

    ::Sleep(time);

    timeEndPeriod(tc.wPeriodMin);
  }

}
