// std library
#include <thread>
#include <vector>

// other libraries
#include <lvgl.h>

// project header files
#include "../power_manager.hpp"
#include "status_bar.hpp"

// GUI objects
static lv_obj_t *status_bar;
static lv_obj_t *battery_status_label;
static std::vector<std::thread> threads; // TODO: need a thread manager.

void drawBatteryPercentage() {
  // draw battery status label
  battery_status_label = lv_label_create(status_bar);
  lv_obj_set_size(battery_status_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_align(battery_status_label, LV_ALIGN_TOP_RIGHT, 0, 0);
  
  static uint32_t battery_percentage; // battery percentage text
  // static char *battery_status_symbol;
  while (true) {
    battery_percentage = PowerManager::instance().getBatteryLevel();
    // printf("[%s] battery_percentage=%d\n", __FUNCTION__, battery_percentage);
    lv_label_set_text_fmt(battery_status_label, "%d%%", battery_percentage);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void drawStatusBar() {
  // status bar style
  static lv_style_t status_bar_style;
  lv_style_init(&status_bar_style);
  lv_style_set_text_font(&status_bar_style, &status_bar_font);
  lv_style_set_border_width(&status_bar_style, 0);
  lv_style_set_outline_width(&status_bar_style, 0);
  lv_style_set_pad_all(&status_bar_style, 0);
  lv_style_set_pad_gap(&status_bar_style, 0);
  lv_style_set_bg_opa(&status_bar_style, LV_OPA_TRANSP);
  
  // draw status bar
  status_bar = lv_obj_create(lv_scr_act());
  lv_obj_set_size(status_bar, kStatusBarWidth, kStatusBarHeight);
  lv_obj_add_style(status_bar, &status_bar_style, 0);
  lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_scrollbar_mode(status_bar, LV_SCROLLBAR_MODE_OFF);

  // draw battery text
  threads.push_back(std::thread(drawBatteryPercentage)); // create a thread to update the battery status
  
}