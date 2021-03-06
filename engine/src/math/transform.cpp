//
// Created by FLXR on 9/15/2019.
//

#include "xepch.hpp"
#include <xe/math/transform.hpp>

namespace xe {

  Transform::Transform() :
      dirty_(true),
      parent_(nullptr),
      position_(0.0f),
      rotation_(0.0f, 0.0f, 0.0f, 1.0f),
      scale_(1.0f),
      worldPosition_(0.0f),
      worldRotation_(0.0f, 0.0f, 0.0f, 1.0f),
      worldScale_(1.0f),
      worldTransform_(mat4::identity()),
      eulerAngles_(0.0f) {
    setName("Transform");
  }

  void Transform::setLocalPosition(const vec3 &position) {
    position_ = position;
    markForUpdate();
  }

  void Transform::setLocalPositionX(float x) {
    position_.set(0, x);
    markForUpdate();
  }

  void Transform::setLocalPositionY(float y) {
    position_.set(1, y);
    markForUpdate();
  }

  void Transform::setLocalPositionZ(float z) {
    position_.set(2, z);
    markForUpdate();
  }

  void Transform::setLocalRotation(const quat &rotation) {
    rotation_ = rotation;
    markForUpdate();
  }

  void Transform::setLocalRotation(const vec3 &rotation) {
    setLocalRotation(quat(rotation));
  }

  void Transform::setLocalScale(float scale) {
    setLocalScale(vec3(scale));
  }

  void Transform::setLocalScale(const vec3 &scale) {
    scale_ = scale;
    markForUpdate();
  }

  void Transform::setTransform(const vec3 &position, const quat &rotation, const vec3 &scale) {
    position_ = position;
    rotation_ = rotation;
    scale_ = scale;
    markForUpdate();
  }

  void Transform::setTransform(const vec3 &position, const vec3 &rotation, const vec3 &scale) {
    setTransform(position, quat(rotation), scale);
  }

  void Transform::setTransform(const vec3 &position, const quat &rotation) {
    position_ = position;
    rotation_ = rotation;
    markForUpdate();
  }

  void Transform::setTransform(const vec3 &position, const vec3 &rotation) {
    setTransform(position, quat(rotation));
  }

  void Transform::setTransform(const mat4 &transform) {
    setTransform(vec3(transform.getTranslation()), transform.getRotation(), vec3(transform.getScale()));
  }

  void Transform::setWorldPosition(const vec3 &position) {
    if (!parent_) {
      setLocalPosition(position);
    } else {
      setLocalPosition(parent_->worldTransform().inverse() * position);
    }
  }

  void Transform::setWorldRotation(const quat &rotation) {
    if (!parent_) {
      setLocalRotation(rotation);
    } else {
      setLocalRotation(parent_->worldRotation().inverse() * rotation);
    }
  }

  void Transform::setWorldScale(float scale) {
    setWorldScale(vec3(scale));
  }

  void Transform::setWorldScale(const vec3 &scale) {
    if (!parent_) {
      setLocalScale(scale);
    } else {
      setLocalScale(scale / parent_->worldScale());
    }
  }

  void Transform::setWorldTransform(const vec3 &position, const quat &rotation, const vec3 &scale) {
    setWorldPosition(position);
    setWorldRotation(rotation);
    setWorldScale(scale);
  }

  void Transform::setWorldTransform(const vec3 &position, const vec3 &rotation, const vec3 &scale) {
    setWorldTransform(position, quat(rotation), scale);
  }

  void Transform::setWorldTransform(const vec3 &position, const quat &rotation) {
    setWorldPosition(position);
    setWorldRotation(rotation);
  }

  void Transform::setWorldTransform(const vec3 &position, const vec3 &rotation) {
    setWorldTransform(position, quat(rotation));
  }

  void Transform::translate(const vec3 &delta, Space space) {
    switch (space) {
      case Space::Local: {
        position_ += rotation_ * delta;
        break;
      }
      case Space::Parent: {
        position_ += delta;
        break;
      }
      case Space::World: {
        if (!parent_) {
          position_ += delta;
        } else {
          position_ += parent_->worldTransform() * delta;
        }
        break;
      }
    }
    markForUpdate();
  }

  void Transform::translateX(float delta, Space space) {
    translate(vec3(1.0f, 0.0f, 0.0f) * delta, space);
  }

  void Transform::translateY(float delta, Space space) {
    translate(vec3(0.0f, 1.0f, 0.0f) * delta, space);
  }

  void Transform::translateZ(float delta, Space space) {
    translate(vec3(0.0f, 0.0f, 1.0f) * delta, space);
  }

  void Transform::rotate(const quat &delta, Space space) {
    switch (space) {
      case Space::Local: {
        rotation_ = (rotation_ * delta).normalize();
        break;
      }
      case Space::Parent: {
        rotation_ = (delta * rotation_).normalize();
        break;
      }
      case Space::World: {
        if (!parent_) {
          rotation_ = (delta * rotation_).normalize();
        } else {
          const quat wr = parent_->worldRotation();
          rotation_ *= wr.inverse() * delta * wr;
        }
        break;
      }
    }
    markForUpdate();
  }

  void Transform::rotate(const vec3 &delta, Space space) {
    rotate(quat(delta), space);
  }

  void Transform::rotateX(float angle, Space space) {
    rotate(quat(vec3(1.0f, 0.0f, 0.0f) * angle), space);
  }

  void Transform::rotateY(float angle, Space space) {
    rotate(quat(vec3(0.0f, 1.0f, 0.0f) * angle), space);
  }

  void Transform::rotateZ(float angle, Space space) {
    rotate(quat(vec3(0.0f, 0.0f, 1.0f) * angle), space);
  }

  void Transform::rotateAround(const vec3 &point, const quat &delta, Space space) {
    vec3 psp;
    quat pr = rotation_;
    switch (space) {
      case Space::Local: {
        psp = localTransform() * point;
        rotation_ = (rotation_ * delta).normalize();
        break;
      }
      case Space::Parent: {
        psp = point;
        rotation_ = (delta * rotation_).normalize();
        break;
      }
      case Space::World: {
        if (!parent_) {
          psp = point;
          rotation_ = (delta * rotation_).normalize();
        } else {
          psp = parent_->worldTransform().inverse() * point;
          const quat wr = parent_->worldRotation();
          rotation_ = rotation_ * wr.inverse() * delta * wr;
        }
        break;
      }
    }
    const vec3 prp = pr.inverse() * (position_ - psp);
    position_ = rotation_ * prp + psp;
    markForUpdate();
  }

  void Transform::rotateAround(const vec3 &point, const vec3 &delta, Space space) {
    rotateAround(point, quat(delta), space);
  }

  void Transform::rotateAround(const Transform &other, const quat &delta, Space space) {
    rotateAround(other.worldPosition(), delta, space);
  }

  void Transform::rotateAround(const Transform &other, const vec3 &delta, Space space) {
    rotateAround(other.worldPosition(), quat(delta), space);
  }

  void Transform::lookAt(const vec3 &target, const vec3 &up, Space space) {
    vec3 tws;

    switch (space) {
      case Space::Local: {
        tws = worldTransform() * target;
        break;
      }
      case Space::Parent: {
        if (!parent_) {
          tws = target;
        } else {
          tws = parent_->worldTransform() * target;
        }
        break;
      }
      case Space::World: {
        tws = target;
        break;
      }
    }

    const vec3 lookDirection = tws - worldPosition();
    quat newRotation = quat::lookRotation(lookDirection, up);

    setWorldRotation(newRotation);
  }

  const vec3 &Transform::localPosition() const {
    return position_;
  }

  const quat &Transform::localRotation() const {
    return rotation_;
  }

  vec3 Transform::localRotationAngles() const {
    return rotation_.toEulerAngles();
  }

  const vec3 &Transform::localScale() const {
    return scale_;
  }

  vec3 Transform::localForward() const {
    return rotation_ * vec3::unitZN();
  }

  vec3 Transform::localRight() const {
    return rotation_ * vec3::unitX();
  }

  vec3 Transform::localUp() const {
    return rotation_ * vec3::unitY();
  }

  mat4 Transform::localTransform() const {
    return mat4::transformation(position_, rotation_, scale_);
  }

  const vec3 &Transform::worldPosition() const {
    if (hasChanged()) {
      updateWorldTransform();
    }
    return worldPosition_;
  }

  const quat &Transform::worldRotation() const {
    if (hasChanged()) {
      updateWorldTransform();
    }
    return worldRotation_;
  }

  vec3 Transform::worldRotationAngles() const {
    return worldRotation().toEulerAngles();
  }

  const vec3 &Transform::worldScale() const {
    if (hasChanged()) {
      updateWorldTransform();
    }
    return worldScale_;
  }

  vec3 Transform::worldForward() const {
    if (hasChanged()) {
      updateWorldTransform();
    }
    return worldRotation_ * vec3::unitZN();
  }

  vec3 Transform::worldRight() const {
    if (hasChanged()) {
      updateWorldTransform();
    }
    return worldRotation_ * vec3::unitX();
  }

  vec3 Transform::worldUp() const {
    if (hasChanged()) {
      updateWorldTransform();
    }
    return worldRotation_ * vec3::unitY();
  }

  mat4 Transform::worldTransform() const {
    if (hasChanged()) {
      updateWorldTransform();
    }
    return worldTransform_;
  }

  void Transform::setParent(const std::shared_ptr<Transform> &parent) {
    if (!parent) {
      return;
    }

    if (parent_ != parent) {
      parent_ = parent;
      std::shared_ptr<Transform> t;
      t.reset(this);
      parent_->children_.push_back(t);

      const mat4 prev = worldTransform();

      setTransform(vec3(prev.getTranslation()), prev.getRotation(), vec3(prev.getScale()));
    }
  }

  const std::shared_ptr<Transform> &Transform::parent() const {
    return parent_;
  }

  const std::shared_ptr<Transform> &Transform::child(uint32_t index) const {
    static const std::shared_ptr<Transform> empty;
    if (index > children_.size() - 1) {
      XE_CORE_ERROR("[Transform] Could not find child with index {} (index out of range). Returning nullptr.", index);
      return empty;
    }
    return children_[index];
  }

  uint32_t Transform::childrenSize() const {
    return static_cast<uint32_t>(children_.size());
  }

  void Transform::updateWorldTransform() const {
    XE_TRACE_BEGIN("XE", "Compute transformations");
    const mat4 transform = localTransform();
    if (!parent_) {
      worldTransform_ = transform;
      worldPosition_ = position_;
      worldRotation_ = rotation_;
      worldScale_ = scale_;
    } else {
      worldTransform_ = parent_->worldTransform() * transform;
      worldPosition_ = vec3(worldTransform_.getTranslation());
      worldRotation_ = parent_->worldRotation() * rotation_;
      worldScale_ = parent_->worldScale() * scale_;
    }
    dirty_ = false;
    XE_TRACE_END("XE", "Compute transformations");
  }

  void Transform::markForUpdate() const {
    if (dirty_) {
      return;
    }

    dirty_ = true;
    for (auto &&c : children_) {
      c->markForUpdate();
    }
  }

  bool Transform::hasChanged() const {
    return dirty_;
  }

}
