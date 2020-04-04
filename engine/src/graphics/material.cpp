//
// Created by trbflxr on 4/3/2020.
//

#include "xepch.hpp"
#include <xe/graphics/material.hpp>

namespace xe {

  void Material::setup() {
    if (gpu_.diffuse) {
      gpu_.diffuse->setup();
    }
    if (gpu_.normals) {
      gpu_.normals->setup();
    }
  }

  void Material::destroy() {
    if (gpu_.diffuse) {
      gpu_.diffuse->destroy();
    }
    if (gpu_.normals) {
      gpu_.normals->destroy();
    }
  }

  void Material::update() {
    if (updated_) {
      return;
    }
    //textures are bindless so it works fine
    bool upd = true;
    if (gpu_.diffuse) {
      const uint64_t handle = gpu_.diffuse->handle();
      gpu_.data.diffuse = *reinterpret_cast<const vec2u *>(&handle);
      upd = handle;
    }
    if (gpu_.normals && upd) {
      const uint64_t handle = gpu_.normals->handle();
      gpu_.data.normals = *reinterpret_cast<const vec2u *>(&handle);
      upd = handle;
    }

    updated_ = upd;
  }

}
