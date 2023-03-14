// 模拟实现 C# 的多播委托
#pragma once

#include <concepts>
#include <functional>
#include <list>
#include <map>
#include <memory>

namespace cwheel {

class DelegateBase {
 public:
  DelegateBase() = default;
  virtual ~DelegateBase() = default;
};

template <typename... Args>
class Delegate : public DelegateBase {
 public:
  using Callable = std::function<Args...>;  // func arg

  explicit Delegate(Callable&& cb) : caller_(std::forward<Callable>(cb)) {}

  ~Delegate() override = default;

  // call args
  template <typename... CallArgs>
    requires std::invocable<Callable, CallArgs...>
  inline void operator()(CallArgs&&... args) const {
    std::invoke(caller_, std::forward<CallArgs>(args)...);
  }

 private:
  Callable caller_;
};

class IDelegatable {
 public:
  // need a base class here
  using DelegateList = std::list<std::unique_ptr<DelegateBase>>;
  using DelegatePos = DelegateList::iterator;
  using DelegatePool = std::map<size_t, DelegateList>;

  IDelegatable() = default;
  virtual ~IDelegatable() = default;

  template <typename Delegatable>
  inline DelegateList& GetDelegateList() {
    size_t delegate_type_code = typeid(Delegatable).hash_code();
    return GetDelegatePool()[delegate_type_code];
  }

  //  here Args is callable func
  template <typename Delegatable, typename... Args>
  inline DelegatePos Register(Args&&... args) {
    using DelegateFuncType = ::cwheel::Delegate<typename Delegatable::Signature>;

    DelegateList& list = GetDelegateList<Delegatable>();

    list.emplace_back(std::make_unique<DelegateFuncType>(std::forward<Args>(args)...));
    return std::prev(list.end());
  }

  template <typename Delegatable>
  inline DelegatePos Delete(DelegatePos pos) {
    DelegateList& list = GetDelegateList<Delegatable>();
    return list.erase(pos);
  }

  template <typename Delegatable>
  inline void Clear() {
    size_t delegate_type_code = typeid(Delegatable).hash_code();
    GetDelegatePool().erase(delegate_type_code);
  }

  // Here Args are real run args
  template <typename Delegatable, typename... Args>
  inline void Invoke(Args&&... args) {
    using DelegateFuncType = ::cwheel::Delegate<typename Delegatable::Signature>;
    size_t delegate_type_code = typeid(Delegatable).hash_code();
    const auto& pool = GetDelegatePool();

    const auto it = pool.find(delegate_type_code);
    if (it != pool.end()) {
      for (const auto& func : it->second) {
        std::invoke(*dynamic_cast<DelegateFuncType*>(func.get()), std::forward<Args>(args)...);
      }
    }
  }

  virtual DelegatePool& GetDelegatePool() = 0;
  [[nodiscard]] virtual const DelegatePool& GetDelegatePool() const = 0;
};

class DefaultDelegateHost : public IDelegatable {
 public:
  DelegatePool& GetDelegatePool() override { return pool_; }

  [[nodiscard]] const DelegatePool& GetDelegatePool() const override { return pool_; }

 private:
  DelegatePool pool_;
};

}  // namespace cwheel