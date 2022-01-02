// std library
#include <chrono>
#include <cstdio>
#include <thread>

// other libraries
#include <lvgl.h>

// project header files
#include "../power_manager.hpp"
#include "battery_status.hpp"

void drawBatteryPercentage() {
  // battery percentage text
  static uint32_t battery_percentage;
  static lv_obj_t *battery_percentage_text = lv_label_create(lv_scr_act());
  lv_obj_align(battery_percentage_text, LV_ALIGN_TOP_RIGHT, -5, 5);
  while (true) {
    battery_percentage = PowerManager::instance().getBatteryLevel();
    printf("[%s] battery_percentage=%d\n", __FUNCTION__, battery_percentage);
    lv_label_set_text_fmt(battery_percentage_text, "%d%%", battery_percentage);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}