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
static lv_obj_t *menu;

void hideScannedWifi() {
  lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
}

void showScannedWifi() {
  lv_obj_clear_flag(menu, LV_OBJ_FLAG_HIDDEN);
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

  // menu style
  static lv_style_t menu_style;
  lv_style_init(&menu_style);
  lv_style_set_bg_opa(&menu_style, LV_OPA_TRANSP);
  lv_style_set_text_font(&menu_style, &lv_font_montserrat_18);

  // draw the menu
  menu = lv_menu_create(lv_scr_act());
  lv_obj_set_size(menu, LV_HOR_RES_MAX, LV_VER_RES_MAX);
  lv_obj_add_style(menu, &menu_style, 0);
  lv_obj_center(menu);

  // draw scanned wifi page
  lv_obj_t *scanned_wifi_page = lv_menu_page_create(menu, "Scanned WiFi");
  // TODO: need to take the min value
  lv_obj_t *cont, *label;
  // draw connect page
  lv_obj_t *connect_page = lv_menu_page_create(menu, "Connect");
  label = lv_label_create(connect_page);
  lv_label_set_text(label, "Not implemented yet");
  std::string str;
  for (int i = 0; i < kMaxScannedWifi; ++i) {
    // draw each entry in the menu
    cont = lv_menu_cont_create(scanned_wifi_page);
    label = lv_label_create(cont);
    lv_label_set_text_fmt(label, "WiFi %d", i);
    // draw the connection page
    str = "WiFi" + std::to_string(i);
    // TODO:
    // pass the wifi name to the page 
    // add the keyboard
    // add text area for password
    lv_menu_set_load_page_event(menu, scanned_wifi_page, connect_page);
  }

  lv_menu_set_page(menu, scanned_wifi_page);
}