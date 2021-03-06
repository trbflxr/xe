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

    inline Timestep operator+(const Timestep &o) const { return Timestep(time_ + o.time_); }
    inline Timestep &operator+=(const Timestep &o) { time_ += o.time_; return *this; }

    inline Timestep operator-(const Timestep &o) const { return Timestep(time_ - o.time_); }
    inline Timestep &operator-=(const Timestep &o) { time_ -= o.time_; return *this; }

    inline Timestep operator*(const Timestep &o) const { return Timestep(time_ * o.time_); }
    inline Timestep &operator*=(const Timestep &o) { time_ *= o.time_; return *this; }

    inline Timestep operator*(float value) const { return Timestep(time_ * value); }
    inline Timestep &operator*=(float value) { time_ *= value; return *this; }

    inline Timestep operator/(const Timestep &o) const { return Timestep(time_ / o.time_); }
    inline Timestep &operator/=(const Timestep &o) { time_ /= o.time_; return *this; }

    inline Timestep operator/(float value) const { return Timestep(time_ / value); }
    inline Timestep &operator/=(float value) { time_ /= value; return *this; }

    inline bool operator==(const Timestep &o) const { return time_ == o.time_; }
    inline bool operator!=(const Timestep &o) const { return time_ != o.time_; }

  private:
    float time_;
  };

}

#endif //XE_TIMESTEP_HPP
