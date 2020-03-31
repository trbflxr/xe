//
// Created by trbflxr on 3/29/2020.
//

#include "xepch.hpp"
#include <xe/graphics/model.hpp>

#include <utils/model_loader.hpp>

namespace xe {

  Model::Model(std::string_view file) {
    ModelLoader::load(file, this);
  }

}
