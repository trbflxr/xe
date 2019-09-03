//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_OBJECT_HPP
#define XE_OBJECT_HPP

#include <xe/common.hpp>
#include <xe/string.hpp>

namespace xe {

#define XE_OBJECT(typeName, baseTypeName)                                 \
public:                                                                   \
  using ClassName = typeName;                                             \
  using BaseClassName = baseTypeName;                                     \
  inline const char* type() const override { return #typeName; }          \
  inline const char* baseType() const override { return #baseTypeName; }  \

  class XE_API Object {
  public:
    Object();
    virtual ~Object() = default;

    inline void setName(const char *name) { name_ = name; }
    inline const string &name() const { return name_; }

    inline uint id() const { return id_; }

    virtual const char *type() const = 0;
    virtual const char *baseType() const = 0;

  private:
    string name_;
    uint id_;
  };

}

#endif //XE_OBJECT_HPP
