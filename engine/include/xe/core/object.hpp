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
  std::string_view type() const override { return #typeName; }            \
  std::string_view baseType() const override { return #baseTypeName; }    \

  class XE_API Object {
  public:
    Object();
    virtual ~Object() = default;

    void setName(std::string_view name) { name_ = name; }
    std::string_view name() const { return name_; }

    uint32_t id() const { return id_; }

    virtual std::string_view type() const = 0;
    virtual std::string_view baseType() const = 0;

  private:
    std::string name_;
    uint32_t id_;
  };

}

#endif //XE_OBJECT_HPP
