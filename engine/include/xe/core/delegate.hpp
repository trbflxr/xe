//
// Created by trbflxr on 3/12/2020.
//

#ifndef XE_DELEGATE_HPP
#define XE_DELEGATE_HPP

#include <algorithm>
#include <functional>
#include <mutex>
#include <memory>
#include <vector>
#include <xe/utils/noncopyable.hpp>

namespace xe {
  template<typename>
  class Delegate;

  struct XE_API Observer {
    std::shared_ptr<bool> valid;

    Observer() : valid(std::make_shared<bool>(true)) { }
    virtual ~Observer() = default;
  };

  template<typename TReturnType, typename ...TArgs>
  class XE_API Invoker {
  public:
    using ReturnType = std::vector<TReturnType>;

    static ReturnType invoke(Delegate<TReturnType(TArgs ...)> &delegate, TArgs ... params) {
      std::lock_guard<std::mutex> lock(delegate.mutex_);
      ReturnType returnValues;

      for (auto it = delegate.functions_.begin(); it != delegate.functions_.end();) {
        if (it->isExpired()) {
          it = delegate.functions_.erase(it);
          continue;
        }
        returnValues.emplace_back((*it->functions_)(params...));
        ++it;
      }
      return returnValues;
    }
  };

  template<typename... TArgs>
  class XE_API Invoker<void, TArgs...> {
  public:
    using ReturnType = void;

    static void invoke(Delegate<void(TArgs ...)> &delegate, TArgs ... params) {
      std::lock_guard<std::mutex> lock(delegate.mutex_);

      if (delegate.functions_.empty()) {
        return;
      }

      for (auto it = delegate.functions_.begin(); it != delegate.functions_.end();) {
        if (it->isExpired()) {
          it = delegate.functions_.erase(it);
          continue;
        }

        it->function(params...);
        ++it;
      }
    }
  };

  template<typename TReturnType, typename ...TArgs>
  class XE_API Delegate<TReturnType(TArgs ...)> {
  public:
    using Invoker = Invoker<TReturnType, TArgs...>;
    using FunctionType = std::function<TReturnType(TArgs ...)>;
    using ObserversType = std::vector<std::weak_ptr<bool>>;

    struct FunctionPair {
      FunctionType function;
      ObserversType observers;

      bool isExpired() {
        for (const auto &observer : observers) {
          if (observer.expired()) {
            return true;
          }
        }
        return false;
      }
    };

  public:
    Delegate() = default;
    virtual ~Delegate() = default;

    template<typename ...KArgs>
    void add(FunctionType &&function, KArgs ...args) {
      std::lock_guard<std::mutex> lock(mutex_);
      ObserversType observers;

      if constexpr (sizeof...(args) != 0) {
        for (const auto &arg : {args...}) {
          observers.emplace_back(to_address(arg)->valid);
        }
      }

      functions_.emplace_back(FunctionPair{std::move(function), observers});
    }

    void remove(const FunctionType &function) {
      std::lock_guard<std::mutex> lock(mutex_);
      functions_.erase(std::remove_if(functions_.begin(), functions_.end(), [function](FunctionPair &f) {
        return hash(f.function) == hash(function);
      }), functions_.end());
    }

    void clear() {
      std::lock_guard<std::mutex> lock(mutex_);
      functions_.clear();
    }

    Delegate &operator+=(FunctionType &&function) {
      return add(std::move(function));
    }

    Delegate &operator-=(const FunctionType function) {
      return remove(function);
    }

    typename Invoker::ReturnType invoke(TArgs ... args) {
      return Invoker::invoke(*this, args...);
    }

    typename Invoker::ReturnType operator()(TArgs ... args) {
      return Invoker::invoke(*this, args...);
    }

  private:
    friend Invoker;

    static constexpr size_t hash(const FunctionType &function) {
      return function.target_type().hash_code();
    }

  private:
    std::mutex mutex_;
    std::vector<FunctionPair> functions_;
  };

  template<typename T>
  class XE_API DelegateValue : public Delegate<void(T)>, NonCopyable {
  public:
    template<typename ...Args>
    DelegateValue(Args ...args) :
        value_(std::forward<Args>(args)...) {
    }

    ~DelegateValue() override = default;

    DelegateValue &operator=(T value) {
      value_ = value;
      invoke(value_);
      return *this;
    }

    explicit operator const T &() const noexcept { return value_; }

    const T &get() const { return value_; }
    const T &operator*() const { return value_; }
    const T *operator->() const { return &value_; }

  protected:
    T value_;
  };
}

#endif //XE_DELEGATE_HPP
