//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_TIMESTEP_HPP
#define XE_TIMESTEP_HPP

namespace xe {

  class Timestep {
  public:
    inline Timestep(float time = 0.0f) : time_(time) { }

    inline operator float() const { return time_; }

    inline float seconds() const { return time_; }
    inline float mills() const { return time_ * 1000.0f; }

  private:
    float time_;
  };

}

#endif //XE_TIMESTEP_HPP
