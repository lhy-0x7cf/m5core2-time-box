// std library

// other libraries

// project header files
#include "lvgl_manager.hpp"
#include "power_manager.hpp"

extern "C" void app_main(void) {
  PowerManager::instance().setup();
  LvglManager::instance().init();
}
