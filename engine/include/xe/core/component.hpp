//
// Created by FLXR on 9/15/2019.
//

#ifndef XE_COMPONENT_HPP
#define XE_COMPONENT_HPP

#include <xe/memory.hpp>
#include <xe/core/object.hpp>
#include <xe/core/timestep.hpp>

namespace xe {

  class Scene;
  class GameObject;
  class Transform;

  class XE_API Component : public Object {
  XE_OBJECT(Component, Object);
    friend class GameObject;
  public:
    Component();

    inline virtual const ref_ptr<GameObject> &gameObject() { return gameObject_; }
    inline virtual const ref_ptr<Transform> &transform() { return transform_; }

    template<class T>
    const ref_ptr<T> &getComponent() { //todo: test
      ref_ptr<T> component;
      for (auto &&c : gameObject_->getComponents()) {
        if ((component = dynamic_cast<T *>(c.get()))) {
          return component;
        }
      }
      return nullptr;
    }

  private:
    ref_ptr<GameObject> gameObject_;
    ref_ptr<Transform> transform_;
  };

  namespace System {
    class XE_API ComponentSystem : public Object {
    XE_OBJECT(ComponentSystem, Object);
    public:
      ComponentSystem();

      virtual void init() { };
      virtual void start() { };
      virtual void preUpdate() { };
      virtual void update(Timestep ts) { };
      virtual void postUpdate() { };
      virtual void renderPreUpdate() { };
      virtual void renderUpdate() { };
      virtual void renderPostUpdate() { };
      virtual void stop() { };

      virtual void onSceneChanged();

    protected:
      ref_ptr<Scene> scene_;
    };

    template<class T>
    class Getter { };
  }

#define XE_COMPONENT_SYSTEM(typeName, baseTypeName)          \
  XE_OBJECT(System::##typeName, System::##baseTypeName);     \
  template<typename T>                                       \
  ref_ptr<T> createInstance() {                              \
    ref_ptr<T> c = make_ref<T>();                            \
    components_.push_back(c);                                \
    return c;                                                \
  }                                                          \
  private:                                                   \
  vector<ref_ptr<xe::##typeName>> components_

}

#endif //XE_COMPONENT_HPP
