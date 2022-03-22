#include "thread_pool.h"

using namespace cwheel;

ThreadPool::~ThreadPool() {
  over_ = true;
  for (auto& worker : workers_) {
    if (worker.joinable()) {
      consumer_.Up();
      worker.join();
    }
  }
}

void ThreadPool::AddTask(Task&& t) {
  producer_.Down();
  task_queue_.Push(std::forward<Task>(t));
  consumer_.Up();
}

void ThreadPool::WorkerThread() {
  while (!over_ && !task_queue_.IsEmpty()) {
    consumer_.Down();
    const auto task = task_queue_.Pop();
    if (task.has_value()) {
      task.value()();
      producer_.Up();
    } else {
      consumer_.Up();
    }
  }
}
