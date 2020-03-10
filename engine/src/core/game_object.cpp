//
// Created by FLXR on 9/15/2019.
//

#include "xepch.hpp"
#include <xe/core/game_object.hpp>

namespace xe {

  GameObject::GameObject() {
    setName("GameObject");
    transform_ = System::Getter<Transform>::get()->createInstance<Transform>();
    transform_->transform_ = transform_;
    std::shared_ptr<GameObject> g;
    g.reset(this);
    transform_->gameObject_ = g;
    components_.push_back(transform_);
  }

  void GameObject::setActive(bool active) {
    active_ = active;
    for (size_t i = 0; i < transform_->childrenSize(); i++) {
      transform_->child(i)->gameObject()->setActive(active);
    }
  }

}
