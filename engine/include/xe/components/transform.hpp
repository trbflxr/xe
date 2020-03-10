//
// Created by FLXR on 9/15/2019.
//

#ifndef XE_TRANSFORM_HPP
#define XE_TRANSFORM_HPP

#include <xe/core/component.hpp>

namespace xe {

  class XE_API Transform : public Component {
  XE_OBJECT(Transform, Component);
  public:
    Transform();

    void setLocalPosition(const vec3 &position);
    void setLocalPositionX(float x);
    void setLocalPositionY(float y);
    void setLocalPositionZ(float z);
    void setLocalRotation(const quat &rotation);
    void setLocalRotation(const vec3 &rotation);
    void setLocalScale(float scale);
    void setLocalScale(const vec3 &scale);

    void setTransform(const vec3 &position, const quat &rotation, const vec3 &scale);
    void setTransform(const vec3 &position, const vec3 &rotation, const vec3 &scale);
    void setTransform(const vec3 &position, const quat &rotation);
    void setTransform(const vec3 &position, const vec3 &rotation);
    void setTransform(const mat4 &transform);

    void setWorldPosition(const vec3 &position);
    void setWorldRotation(const quat &rotation);
    void setWorldScale(float scale);
    void setWorldScale(const vec3 &scale);

    void setWorldTransform(const vec3 &position, const quat &rotation, const vec3 &scale);
    void setWorldTransform(const vec3 &position, const vec3 &rotation, const vec3 &scale);
    void setWorldTransform(const vec3 &position, const quat &rotation);
    void setWorldTransform(const vec3 &position, const vec3 &rotation);

    void translate(const vec3 &delta, TransformSpace space = TransformSpace::Local);
    void translateX(float delta, TransformSpace space = TransformSpace::Local);
    void translateY(float delta, TransformSpace space = TransformSpace::Local);
    void translateZ(float delta, TransformSpace space = TransformSpace::Local);

    void rotate(const quat &delta, TransformSpace space = TransformSpace::Local);
    void rotate(const vec3 &delta, TransformSpace space = TransformSpace::Local);
    void rotateX(const float &angle, TransformSpace space = TransformSpace::Local);
    void rotateY(const float &angle, TransformSpace space = TransformSpace::Local);
    void rotateZ(const float &angle, TransformSpace space = TransformSpace::Local);

    void rotateAround(const vec3 &point, const quat &delta, TransformSpace space = TransformSpace::Local);
    void rotateAround(const vec3 &point, const vec3 &delta, TransformSpace space = TransformSpace::Local);
    void rotateAround(const std::shared_ptr<GameObject> &obj, const quat &delta, TransformSpace space = TransformSpace::Local);
    void rotateAround(const std::shared_ptr<GameObject> &obj, const vec3 &delta, TransformSpace space = TransformSpace::Local);

    void lookAt(const vec3 &target, const vec3 &up = vec3::unitY(), TransformSpace space = TransformSpace::World);

    const vec3 &localPosition() const;
    const quat &localRotation() const;
    vec3 localRotationAngles() const;
    const vec3 &localScale() const;
    vec3 localForward() const;
    vec3 localRight() const;
    vec3 localUp() const;

    mat4 localTransform() const;

    const vec3 &worldPosition() const;
    const quat &worldRotation() const;
    vec3 worldRotationAngles() const;
    const vec3 &worldScale() const;
    vec3 worldForward() const;
    vec3 worldRight() const;
    vec3 worldUp() const;

    mat4 worldTransform() const;

    void setParent(const std::shared_ptr<Transform> &parent);
    const std::shared_ptr<Transform> &parent() const;
    const std::shared_ptr<Transform> &child(uint index = 0) const;
    uint childrenSize() const;

    void updateWorldTransform() const;

    void markForUpdate() const;
    bool hasChanged() const;

  private:
    mutable bool dirty_;

    std::shared_ptr<Transform> parent_;
    std::vector<std::shared_ptr<Transform>> children_;

    vec3 position_;
    quat rotation_;
    vec3 scale_;

    mutable vec3 worldPosition_;
    mutable quat worldRotation_;
    mutable vec3 worldScale_;

    mutable mat4 worldTransform_;

    vec3 eulerAngles_;
  };

  namespace System {
    class XE_API Transform : public ComponentSystem {
    XE_COMPONENT_SYSTEM(Transform, ComponentSystem);
    public:
      Transform();

      void preUpdate() override;
    };

    template<>
    class Getter<xe::Transform> {
    public:
      static const std::shared_ptr<System::Transform> &get();
    };

  }

}

#endif //XE_TRANSFORM_HPP
