#pragma once

#include <algorithm>
#include <type_traits>
#include <utility>

namespace cwheel {

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
  Functor f_;

 public:
  template <typename FunctorFwd>
  explicit SpecificFunctionBridge(FunctorFwd&& functor) : f_(std::forward<FunctorFwd>(functor)) {}

  virtual ~SpecificFunctionBridge() = default;
  virtual ReturnType Invoke(Args... args) { return f_(std::forward<Args>(args)...); }
  virtual SpecificFunctionBridge* Clone() const { return new SpecificFunctionBridge(f_); }
};

template <typename Signature>
class FunctionPtr;

template <typename ReturnType, typename... Args>
class FunctionPtr<ReturnType(Args...)> {
 private:
  FunctionBridge<ReturnType, Args...>* bridge_;

 public:
  // constructors:
  FunctionPtr() : bridge_(nullptr) {}
  FunctionPtr(FunctionPtr const& other) {
    if (other.bridge_) {
      bridge_ = other.bridge_->Clone();
    }
  }
  FunctionPtr(FunctionPtr& other) : FunctionPtr(static_cast<FunctionPtr const&>(other)) {}
  FunctionPtr(FunctionPtr&& other) noexcept : bridge_(other.bridge_) { other.bridge_ = nullptr; }

  // construction from arbitrary function objects:
  template <typename F>
  explicit FunctionPtr(F&& f) {
    // Type Erasure here
    bridge_ = new SpecificFunctionBridge<F, ReturnType, Args...>(std::forward<F>(f));
  }
  FunctionPtr& operator=(FunctionPtr const& other) {
    FunctionPtr tmp(other);
    std::swap(*this, tmp);
    return *this;
  }
  FunctionPtr& operator=(FunctionPtr&& other) noexcept {
    delete bridge_;
    bridge_ = other.bridge_;
    other.bridge_ = nullptr;
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
  ~FunctionPtr() { delete bridge_; }
  friend void swap(FunctionPtr& fp1, FunctionPtr& fp2) { std::swap(fp1.bridge_, fp2.bridge_); }
  explicit operator bool() const { return bridge_ == nullptr; }
  // invocation:
  ReturnType operator()(Args... args) const {
    if (bridge_) {
      bridge_->Invoke(std::forward<Args>(args)...);
    }
  }
};
}  // namespace cwheel