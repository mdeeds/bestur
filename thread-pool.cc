#include "thread-pool.h"

void ThreadPool::join() {
   {
    while (!queue_.empty()) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(mutex_);
        while (active_threads_ >= capacity_) {
          cond_.wait(lock);
        }
        if (queue_.empty()) {
          break;
        }
        task = queue_.front();
        queue_.pop();
      }
      active_threads_++;
      std::thread t([&](){
        task();
        std::unique_lock<std::mutex> lock(mutex_);
        --active_threads_;
        cond_.notify_one();
      });
      t.detach();
    }
  }
}