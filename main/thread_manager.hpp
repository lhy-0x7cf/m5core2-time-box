#ifndef M5CORE2_TIME_BOX_THREAD_MANAGER_HPP_
#define M5CORE2_TIME_BOX_THREAD_MANAGER_HPP_

// std library
#include <thread>
#include <vector>

// other libraries

// project header files

class ThreadManager {
 public:
  template<typename Func, typename... Args>
  void addThread(Func fn, Args... args) {
    threads.push_back(std::thread(fn, args...));
  }

  // enable singleton
  static ThreadManager& instance() {
    static ThreadManager me;
    return me;
  }
  ThreadManager(ThreadManager const&) = delete; // delete copy construct
  ThreadManager(ThreadManager&&) = delete; // delete move construct
  ThreadManager& operator=(ThreadManager const&) = delete; // delete copy assign
  ThreadManager& operator=(ThreadManager &&) = delete; // delete move assign

 private:
  std::vector<std::thread> threads;

  ThreadManager() = default;
  ~ThreadManager() {
    for (int i = 0; i < threads.size(); ++i) {
      if (threads[i].joinable()) {
        threads[i].join();
      }
    }
  }
};

#endif