#ifndef THREAD_POOL
#define THREAD_POOL

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
 public:
  ThreadPool(int capacity) : active_threads_(0), capacity_(capacity) {}

  void enqueue(std::function<void()> task) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(task);
    cond_.notify_one();
  }

  void join();

 private:
  std::queue<std::function<void()>> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
  int active_threads_;
  int capacity_;
};

#endif
