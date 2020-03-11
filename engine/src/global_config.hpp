//
// Created by trbflxr on 3/11/2020.
//

#ifndef XE_GLOBAL_CONFIG_HPP
#define XE_GLOBAL_CONFIG_HPP

#include <xe/params.hpp>
#include <xe/utils/noncopyable.hpp>

namespace xe {

  class GlobalConfig : NonCopyable {
    friend class Engine;
  public:
    static GlobalConfig &ref() {
      static GlobalConfig config;
      return config;
    }

    Params &getParams() { return params_; }
    const Params &getParams() const { return params_; }

  private:
    Params params_;
    bool initialized_ = false;
  };

}

#endif //XE_GLOBAL_CONFIG_HPP
