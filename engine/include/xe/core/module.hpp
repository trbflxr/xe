//
// Created by trbflxr on 3/10/2020.
//

#ifndef XE_MODULE_HPP
#define XE_MODULE_HPP

#include <map>
#include <memory>
#include <xe/common.hpp>
#include <xe/utils/noncopyable.hpp>

namespace xe {

  template<typename Base>
  class ModuleFactory {
  public:
    enum class Stage {
      Never,
      Always,
      Pre,
      Normal,
      Post,
      Render
    };

    using StageIndex = std::pair<Stage, size_t>;
    using TRegistryMap = std::multimap<StageIndex, std::unique_ptr<Base>>;

    virtual ~ModuleFactory() = default;

    static TRegistryMap &registry() {
      static TRegistryMap impl;
      return impl;
    }

    static size_t getNextId() {
      static size_t id = 0;
      return ++id;
    }

    template<typename T, Stage S>
    class Registrar : public Base {
    public:
      static T *get() { return moduleInstance_; }

      static bool registerModule() {
        auto it = registry().insert({StageIndex(S, getNextId()), std::make_unique<T>()});
        moduleInstance_ = dynamic_cast<T *>(it->second.get());
        return true;
      }

      static bool deregisterModule() {
        for (auto it = registry().begin(); it != registry().end();) {
          if (it->second.get() == moduleInstance_) {
            it = registry().erase(it);
          } else {
            ++it;
          }
        }
        moduleInstance_ = nullptr;
        return true;
      }

    private:
      inline static T *moduleInstance_ = nullptr;
    };
  };


  class XE_API Module : public ModuleFactory<Module>, NonCopyable {
  public:
    virtual void update() = 0;
  };

}

#endif //XE_MODULE_HPP
