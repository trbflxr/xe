//
// Created by FLXR on 9/15/2019.
//

#ifndef XE_GAME_OBJECT_HPP
#define XE_GAME_OBJECT_HPP

#include <xe/core/component.hpp>
#include <xe/components/transform.hpp>

namespace xe {

  class XE_API GameObject : public Object {
  XE_OBJECT(GameObject, Object);
    friend class Transform;
  public:
    GameObject();

    inline const std::shared_ptr<Transform> &transform() { return transform_; }

    void setActive(bool active);
    inline bool active() const { return active_; }

    inline uint32_t sceneId() const { return sceneId_; }

    inline const std::vector<std::shared_ptr<Component>> &getComponents() const { return components_; }

    template<class T>
    const std::shared_ptr<T> &getComponent() { //todo: test
      std::shared_ptr<T> component;
      for (auto &&c : components_) {
        if ((component = dynamic_cast<T *>(c.get()))) {
          return component;
        }
      }
      return nullptr;
    }

    template<class T>
    const std::shared_ptr<T> &getInChildrenComponent() { //todo: test
      std::shared_ptr<T> component;
      for (size_t i = 0; i < transform_->childrenSize(); ++i) {
        component = transform_->child(i)->getComponent<T>();
        if (component) {
          return component;
        }
        component = transform_->child(i)->gameObject()->getComponentInChildren<T>();
        if (component) {
          return component;
        }
      }
      return nullptr;
    }

    template<class T, class L=T>
    const std::shared_ptr<T> &addComponent() { //todo: test
      for (auto &&c : components_) {
        auto other = dynamic_cast<T *>(c.get());
        if (other) {
          return other;
        }
      }
      std::shared_ptr<T> component = System::Getter<L>::get()->createInstance<T>();
      component->gameObject_ = this;
      component->transform_ = transform_;
      components_.push_back(component);
      return component;
    }

  private:
    std::shared_ptr<Transform> transform_;
    std::vector<std::shared_ptr<Component>> components_;

    uint32_t sceneId_ = 0;
    bool active_ = true;
  };

}

#endif //XE_GAME_OBJECT_HPP
