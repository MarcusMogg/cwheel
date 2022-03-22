// A simple thread pool using semaphore and blocking queue
#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <optional>
#include <queue>
#include <thread>
namespace cwheel {

class Semaphore {
 private:
  int cnt_;
  std::mutex mutex_;
  std::condition_variable cond_;

 public:
  Semaphore(int n) : cnt_(n) {}

  inline void Down() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this]() { return cnt_ > 0; });
    --cnt_;
  }

  inline void Up() {
    std::lock_guard<std::mutex> lock(mutex_);
    ++cnt_;
    cond_.notify_one();
  }
};

template <typename T>
class BlockingQueue {
 private:
  std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
  std::queue<T> queue_;

 public:
  BlockingQueue() {}
  ~BlockingQueue() = default;

  inline void Push(T&& value) {
    while (std::atomic_flag_test_and_set_explicit(&lock_, std::memory_order_acquire)) {
    }
    queue_.emplace(std::forward<T>(value));
    std::atomic_flag_clear_explicit(&lock_, std::memory_order_release);
  }

  inline std::optional<T> Pop() {
    while (std::atomic_flag_test_and_set_explicit(&lock_, std::memory_order_acquire)) {
    }
    std::optional<T> value;
    if (!queue_.empty()) {
      value = queue_.front();
      queue_.pop();
    }
    std::atomic_flag_clear_explicit(&lock_, std::memory_order_release);
    return value;
  }

  inline bool IsEmpty() {
    while (std::atomic_flag_test_and_set_explicit(&lock_, std::memory_order_acquire)) {
    }
    const bool is_empty = queue_.empty();
    std::atomic_flag_clear_explicit(&lock_, std::memory_order_release);
    return is_empty;
  }
};

class ThreadPool {
 private:
  using Task = std::function<void()>;
  static const uint64_t kBufferSize = 1024;

  std::atomic<bool> over_;

  BlockingQueue<Task> task_queue_;
  std::vector<std::thread> workers_;

  Semaphore producer_;
  Semaphore consumer_;

 public:
  explicit ThreadPool(const uint64_t size) : over_(false), workers_(size), producer_(kBufferSize), consumer_(0) {
    for (auto& worker : workers_) {
      worker = std::thread([this]() { this->WorkerThread(); });
    }
  }
  ~ThreadPool();

  void AddTask(Task&& t);

 private:
  void WorkerThread();
};

}  // namespace cwheel