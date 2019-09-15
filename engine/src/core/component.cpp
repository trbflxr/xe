//
// Created by FLXR on 9/15/2019.
//

#include "xepch.hpp"
#include <xe/core/component.hpp>
#include <xe/core/engine.hpp>

namespace xe {

  Component::Component() {
    setName("Component");
  }

  //system
  System::ComponentSystem::ComponentSystem() {
    setName("ComponentSystem");
  }

  void System::ComponentSystem::onSceneChanged() {
    scene_ = Engine::ref().scene();
  }

}
