//
// Created by FLXR on 9/3/2019.
//

#include <xe/core/object.hpp>

namespace xe {

  Object::Object() {
    static uint id;
    id_ = ++id;
    name_ = "Object";
  }

}
