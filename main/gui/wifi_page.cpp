// std library
#include <string>
#include <type_traits>

// other libraries
#include <lvgl.h>

// project header files
#include "wifi_page.hpp"
#include "nav_page.hpp"

// constants
const uint8_t kMaxScannedWifi = 10;

// GUI objects
static lv_obj_t *scanned_wifi_list;

void hideScannedWifi() {
  lv_obj_add_flag(scanned_wifi_list, LV_OBJ_FLAG_HIDDEN);
}

void showScannedWifi() {
  lv_obj_clear_flag(scanned_wifi_list, LV_OBJ_FLAG_HIDDEN);
}

void drawScannedWifi() {
  static bool is_drawn = false;
  if (is_drawn) {
    showScannedWifi();
    return;
  }
  is_drawn = true;
  
  // add return to nav page call back
  lv_obj_add_event_cb(lv_scr_act(), return_to_nav_page_event_cb, LV_EVENT_GESTURE, (void *) hideScannedWifi);

  // scanned_wifi_list style
  static lv_style_t list_style;
  lv_style_init(&list_style);
  lv_style_set_bg_opa(&list_style, LV_OPA_TRANSP);
  lv_style_set_text_font(&list_style, &lv_font_montserrat_18);

  
}