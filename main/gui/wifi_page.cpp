// std library
#include <string>
#include <type_traits>

// other libraries
#include <lvgl.h>

// project header files
#include "wifi_page.hpp"
#include "nav_page.hpp"
#include "status_bar.hpp"
#include "utils.hpp"

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

static void scan_btn_event_cb(lv_event_t *e) {
  // TODO: implement this
}

static void wifi_btn_event_cb(lv_event_t *e) {
  // TODO: implement this
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

  // constants
  static const uint16_t kListWidth = LV_HOR_RES_MAX;
  static const uint16_t kListHeight = LV_VER_RES_MAX - kStatusBarHeight;

  // scanned_wifi_list style
  static lv_style_t list_style;
  lv_style_init(&list_style);
  // lv_style_set_bg_opa(&list_style, LV_OPA_TRANSP);
  lv_style_set_text_font(&list_style, &lv_font_montserrat_18);

  // draw list
  scanned_wifi_list = lv_list_create(lv_scr_act());
  lv_obj_set_size(scanned_wifi_list, kListWidth, kListHeight);
  lv_obj_align(scanned_wifi_list, LV_ALIGN_BOTTOM_MID, 0, 0);
  
  // draw scan button
  lv_obj_t *btn;

  lv_list_add_text(scanned_wifi_list, "Scan");
  btn = lv_list_add_btn(scanned_wifi_list, LV_SYMBOL_REFRESH, "Scan");
  enablePressLock(btn);
  lv_obj_add_event_cb(btn, scan_btn_event_cb, LV_EVENT_CLICKED, NULL);

  // list scanned wifi
  lv_list_add_text(scanned_wifi_list, "Scanned WiFi");
  // TODO: get the list of scanned wifi from wifi manager
  std::string wifi_name;
  for (int i = 0; i < kMaxScannedWifi; ++i) {
    wifi_name = "WiFi " + std::to_string(i);
    btn = lv_list_add_btn(scanned_wifi_list, LV_SYMBOL_WIFI, wifi_name.c_str());
    enablePressLock(btn);
    lv_obj_add_event_cb(btn, wifi_btn_event_cb, LV_EVENT_CLICKED, NULL);
  }
}