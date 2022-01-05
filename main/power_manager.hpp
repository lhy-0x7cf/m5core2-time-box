#ifndef M5CORE2_TIME_BOX_POWER_MANAGER_HPP_
#define M5CORE2_TIME_BOX_POWER_MANAGER_HPP_

// std library
#include <stdint.h>

// other libraries

// project header files

class PowerManager {
 public:
  void setup();
  void vibrate(bool on);
  uint32_t getBatteryLevel();

  // enable singleton
  static PowerManager& instance() {
    static PowerManager me;
    return me;
  }
  PowerManager(PowerManager const&) = delete; // delete copy construct
  PowerManager(PowerManager&&) = delete; // delete move construct
  PowerManager& operator=(PowerManager const&) = delete; // delete copy assign
  PowerManager& operator=(PowerManager &&) = delete; // delete move assign

 private:
  PowerManager() = default;

};

#endif // M5CORE2_LVGL_DEMO_ESP_IDF_POWER_MANAGER_HPP_