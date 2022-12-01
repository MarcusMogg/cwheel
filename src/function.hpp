#pragma once

#include <algorithm>
#include <type_traits>
#include <utility>

namespace cwheel {

template <typename Signature>
class FunctionPtr;

template <typename ReturnType, typename... Args>
class FunctionBridge {
 public:
  virtual ~FunctionBridge() = default;
  virtual ReturnType Invoke(Args... args) = 0;
  virtual FunctionBridge* Clone() const = 0;
};

template <typename Functor, typename ReturnType, typename... Args>
class SpecificFunctionBridge : public FunctionBridge<ReturnType, Args...> {
 private:
  Functor f;

 public:
  template <typename FunctorFwd>
  SpecificFunctionBridge(FunctorFwd&& functor) : f(std::forward<FunctorFwd>(functor)) {}

  virtual ~SpecificFunctionBridge() = default;
  virtual ReturnType Invoke(Args... args) { return f(std::forward<Args>(args)...); }
  virtual SpecificFunctionBridge* Clone() const { return new SpecificFunctionBridge(f); }
};

template <typename ReturnType, typename... Args>
class FunctionPtr<ReturnType(Args...)> {
 private:
  FunctionBridge<ReturnType, Args...>* bridge;

 public:
  // constructors:
  FunctionPtr() : bridge(nullptr) {}
  FunctionPtr(FunctionPtr const& other) {
    if (other.bridge) {
      bridge = other.bridge->Clone();
    }
  }
  FunctionPtr(FunctionPtr& other) : FunctionPtr(static_cast<FunctionPtr const&>(other)) {}
  FunctionPtr(FunctionPtr&& other) : bridge(other.bridge) { other.bridge = nullptr; }

  // construction from arbitrary function objects:
  template <typename F>
  FunctionPtr(F&& f) {
    using Functor = std::decay_t<F>;
    // Type Erasure here
    bridge = new SpecificFunctionBridge<F, ReturnType, Args...>(std::forward<F>(f));
  }
  FunctionPtr& operator=(FunctionPtr const& other) {
    FunctionPtr tmp(other);
    std::swap(*this, tmp);
    return *this;
  }
  FunctionPtr& operator=(FunctionPtr&& other) {
    delete bridge;
    bridge = other.bridge;
    other.bridge = nullptr;
    return *this;
  }
  // construction and assignment from arbitrary function objects:
  template <typename F>
  FunctionPtr& operator=(F&& f) {
    FunctionPtr tmp(std::forward<F>(f));
    swap(*this, tmp);
    return *this;
  }
  // destructor:
  ~FunctionPtr() { delete bridge; }
  friend void swap(FunctionPtr& fp1, FunctionPtr& fp2) { std::swap(fp1.bridge, fp2.bridge); }
  explicit operator bool() const { return bridge == nullptr; }
  // invocation:
  ReturnType operator()(Args... args) const {
    if (bridge) {
      bridge->Invoke(std::forward<Args>(args)...);
    }
  }
};
}  // namespace cwheel