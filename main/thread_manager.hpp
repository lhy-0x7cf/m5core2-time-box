#ifndef M5CORE2_TIME_BOX_THREAD_MANAGER_HPP_
#define M5CORE2_TIME_BOX_THREAD_MANAGER_HPP_

// std library
#include <thread>
#include <vector>

// other libraries

// project header files

/**
 * NOTE:
 * This thread manager is not really a manager, since I cannot find a way to
 * join threads when the physical shutdown happens. It is more like a simple
 * and naive thread pool to hold threads in one place.
 * 
 * I try the method here: http://cplusplus.com/forum/general/190480/#msg921110.
 * I add an atomic to indicate whether the system is running or not and pass it
 * into some function with an infinite loop like drawBatteryPercentage() to
 * control the lifecycle of the loop. However, the log print after the loop
 * cannot be found after I pressesd the reset button or power button. Besides,
 * this method also bumps up the CPU usage a little bit, so I just choose not
 * to use it.
 */
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
  ~ThreadManager() = default;
};

#endif