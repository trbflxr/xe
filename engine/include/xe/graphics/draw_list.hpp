//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_DRAW_LIST_HPP
#define XE_DRAW_LIST_HPP

#include <xe/memory.hpp>
#include <xe/vector.hpp>
#include <xe/core/object.hpp>
#include <xe/graphics/color.hpp>

namespace xe {

  class XE_API DrawList : public Object {
  XE_OBJECT(DrawList, Object);
    friend class GPU;
  public:
    struct Command;

    explicit DrawList();

    void update();

#define PROPERTY(type, name, ...) \
  type name = __VA_ARGS__;\
  Self& set_##name(const type &c) { name = c; return *this; }

#define PROPERTY_PTR(type, name) \
  const type *name = nullptr;\
  Self& set_##name(const type *c) { name = c; return *this; }

#define PROP_ARRAY(type, count, name) \
  type name[count] = {};\
  Self& set_##name(size_t i, const type &c) { name[i] = c; return *this; }\
  Self& set_v_##name(std::vector<type> c) { for (uint32 i = 0; i < c.size(); ++i) { set_##name(i, c[i]); } return *this; }

    struct ClearData {
      typedef ClearData Self;
      PROPERTY(Color, color, { 0.0f, 0.0f, 0.0f, 1.0f });
      PROPERTY(float, depth, 1.0f);
      PROPERTY(int32, stencil, 0);
      PROPERTY(bool, clearColor, true);
      PROPERTY(bool, clearDepth, true);
      PROPERTY(bool, clearStencil, false);
    };

#undef PROPERTY
#undef PROPERTY_PTR
#undef PROP_ARRAY

    ClearData &clearCommand();

  private:
    vector<ref_ptr<Command>> commands_;

  };

}

#endif //XE_DRAW_LIST_HPP
