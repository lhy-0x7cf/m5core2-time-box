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
static const uint8_t kMaxScannedWifi = 10;

// GUI objects
static lv_obj_t *wifi_list_page;

void hideScannedWifi() {
  lv_obj_remove_event_cb(lv_scr_act(), return_to_nav_page_event_cb);
  lv_obj_add_flag(wifi_list_page, LV_OBJ_FLAG_HIDDEN);
}

void showScannedWifi() {
  lv_obj_add_event_cb(lv_scr_act(), return_to_nav_page_event_cb, LV_EVENT_GESTURE, (void *) hideScannedWifi);
  lv_obj_clear_flag(wifi_list_page, LV_OBJ_FLAG_HIDDEN);
}

static void scan_btn_event_cb(lv_event_t *e) {
  // TODO: implement this
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_LONG_PRESSED) {
    printf("Scan button long pressed\n"); // test mis-click
  }
}

static void wifi_btn_event_cb(lv_event_t *e) {
  // TODO: implement this
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_LONG_PRESSED) {
    printf("WiFi button long pressed\n"); // test mis-click
  }
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
  enablePressLock(lv_scr_act());

  // constants
  static const uint16_t kPageWidth = LV_HOR_RES_MAX;
  static const uint16_t kPageHeight = LV_VER_RES_MAX - kStatusBarHeight;

  // wifi_list_page style
  static lv_style_t page_style;
  lv_style_init(&page_style);
  lv_style_set_bg_opa(&page_style, LV_OPA_TRANSP);
  lv_style_set_border_width(&page_style, 0);
  lv_style_set_outline_width(&page_style, 0);
  lv_style_set_text_font(&page_style, &lv_font_montserrat_18);

  // button style
  static lv_style_t button_style;
  lv_style_set_border_color(&button_style, lv_color_white());
  lv_style_set_border_width(&button_style, 1);
  lv_style_set_outline_width(&button_style, 0);
  lv_style_set_bg_opa(&button_style, LV_OPA_TRANSP);

  // create the page with flex layout
  wifi_list_page = lv_obj_create(lv_scr_act());
  lv_obj_set_size(wifi_list_page, kPageWidth, kPageHeight);
  lv_obj_add_style(wifi_list_page, &page_style, 0);
  lv_obj_align(wifi_list_page, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_flex_flow(wifi_list_page, LV_FLEX_FLOW_COLUMN);
  lv_obj_add_flag(wifi_list_page, LV_OBJ_FLAG_CLICKABLE);

  // draw the page
  lv_obj_t *button, *label;
  // draw the scan button
  button = lv_btn_create(wifi_list_page);
  lv_obj_set_size(button, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_add_style(button, &button_style, 0);
  lv_obj_add_event_cb(button, scan_btn_event_cb, LV_EVENT_LONG_PRESSED, NULL);
  label = lv_label_create(button);
  lv_label_set_text(label, LV_SYMBOL_REFRESH " Scan");
  // draw the list of scanned wifi
  label = lv_label_create(wifi_list_page);
  lv_label_set_text(label, "Scanned WiFi");
  for (int i = 0; i < kMaxScannedWifi; ++i) {
    button = lv_btn_create(wifi_list_page);
    lv_obj_set_size(button, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_add_style(button, &button_style, 0);
    lv_obj_add_event_cb(button, wifi_btn_event_cb, LV_EVENT_LONG_PRESSED, NULL);
    label = lv_label_create(button);
    lv_label_set_text_fmt(label, LV_SYMBOL_WIFI " WiFi %d", i);
  }
}