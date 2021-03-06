// 模拟实现 C# 的多播委托
#pragma once

#include <functional>
#include <list>
#include <map>

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

  ~Delegate() = default;

  // call args
  template <typename... CallArgs>
  inline void operator()(CallArgs&&... args) const {
    caller_(std::forward<CallArgs>(args)...);
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

  template <typename Delegate>
  inline DelegateList& GetDelegateList() {
    size_t delegate_type_code = typeid(Delegate).hash_code();
    return GetDelegatePool()[delegate_type_code];
  }

  //  here Args is callable func
  template <typename Delegate, typename... Args>
  inline DelegatePos Register(Args&&... args) {
    using DelegateFuncType = ::cwheel::Delegate<typename Delegate::Signature>;

    DelegateList& list = GetDelegateList<Delegate>();

    list.emplace_back(std::make_unique<DelegateFuncType>(std::forward<Args>(args)...));
    return std::prev(list.end());
  }

  template <typename Delegate>
  inline DelegatePos Delete(DelegatePos pos) {
    DelegateList& list = GetDelegateList<Delegate>();
    return list.erase(pos);
  }

  template <typename Delegate>
  inline void Clear() {
    size_t delegate_type_code = typeid(Delegate).hash_code();
    GetDelegatePool().erase(delegate_type_code);
  }

  // Here Args are real run args
  template <typename Delegate, typename... Args>
  inline void Invoke(Args&&... args) {
    using DelegateFuncType = ::cwheel::Delegate<typename Delegate::Signature>;
    size_t delegate_type_code = typeid(Delegate).hash_code();
    const auto& pool = GetDelegatePool();

    if (pool.count(delegate_type_code)) {
      for (const auto& func : pool.at(delegate_type_code)) {
        (*dynamic_cast<DelegateFuncType*>(func.get()))(std::forward<Args>(args)...);
      }
    }
  }

  virtual DelegatePool& GetDelegatePool() = 0;
  virtual const DelegatePool& GetDelegatePool() const = 0;
};

class DefaultDelegateHost : public IDelegatable {
 public:
  DelegatePool& GetDelegatePool() override { return pool_; }

  const DelegatePool& GetDelegatePool() const override { return pool_; }

 private:
  DelegatePool pool_;
};

}  // namespace cwheel