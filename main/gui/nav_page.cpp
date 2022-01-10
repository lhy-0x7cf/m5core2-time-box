// std library
#include <string>
#include <vector>
#include <utility>

// other libraries
#include <lvgl.h>

// project header files
#include "countdown_timer.hpp"
#include "nav_page.hpp"
#include "status_bar.hpp"
#include "wifi_page.hpp"

// GUI objects
static lv_obj_t *nav_page;
static std::vector<lv_obj_t *> buttons;

void hideNavPage() {
  lv_obj_add_flag(nav_page, LV_OBJ_FLAG_HIDDEN);
}

void showNavPage() {
  lv_obj_clear_flag(nav_page, LV_OBJ_FLAG_HIDDEN);
}

static void timer_btn_even_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    hideNavPage();
    drawTimePicker();
  }
}

static void local_info_btn_even_cb(lv_event_t *e) {
  // TODO: implement this
}

static void wifi_btn_even_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    hideNavPage();
    drawScannedWifi();
  }
}

static void settings_btn_even_cb(lv_event_t *e) {
  // TODO: implement this
}

static std::vector<std::pair<std::string, lv_event_cb_t>> button_utils {
  {"Timer", timer_btn_even_cb},
  {"Local Info", local_info_btn_even_cb},
  {LV_SYMBOL_WIFI " WiFi", wifi_btn_even_cb},
  {LV_SYMBOL_SETTINGS " Settings", settings_btn_even_cb}
};
static const uint16_t kButtonNumber = button_utils.size();

void drawNavPage() {
  static bool is_drawn = false;
  if (is_drawn) {
    showNavPage();
    return;
  }
  is_drawn = true;

  // constants
  const uint16_t kNavPageWidth = LV_HOR_RES_MAX;
  const uint16_t kNavPageHeight = LV_VER_RES_MAX - kStatusBarHeight;
  const uint16_t kColumnNumber = 2;
  const uint16_t kButtonHeight = 90;

  // nav_page style
  static lv_style_t nav_page_style;
  lv_style_set_border_width(&nav_page_style, 0);
  lv_style_set_outline_width(&nav_page_style, 0);
  lv_style_set_bg_opa(&nav_page_style, LV_OPA_TRANSP);

  // button style
  static lv_style_t button_style;
  lv_style_set_border_color(&button_style, lv_color_white());
  lv_style_set_border_width(&button_style, 1);
  lv_style_set_outline_width(&button_style, 0);
  lv_style_set_bg_opa(&button_style, LV_OPA_TRANSP);

  // draw the nav_page
  nav_page = lv_obj_create(lv_scr_act());
  lv_obj_set_size(nav_page, kNavPageWidth, kNavPageHeight);
  lv_obj_add_style(nav_page, &nav_page_style, 0);
  lv_obj_align(nav_page, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_flex_flow(nav_page, LV_FLEX_FLOW_ROW_WRAP);
  lv_obj_set_scrollbar_mode(nav_page, LV_SCROLLBAR_MODE_ACTIVE);

  // draw buttons
  buttons.resize(kButtonNumber);
  lv_obj_t *label;
  for (int i = 0; i < kButtonNumber; ++i) {
    buttons[i] = lv_btn_create(nav_page);
    lv_obj_add_event_cb(buttons[i], button_utils[i].second, LV_EVENT_CLICKED, NULL);
    // NOTE: This make sure there are just 2 columns
    if (i % kColumnNumber == 0) {
      // NOTE: force to align the item in the new line
      lv_obj_add_flag(buttons[i], LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }
    lv_obj_set_flex_grow(buttons[i], 1); // flex grow the width
    lv_obj_set_height(buttons[i], kButtonHeight);
    lv_obj_add_style(buttons[i], &button_style, 0);
    label = lv_label_create(buttons[i]);
    lv_label_set_text(label, button_utils[i].first.c_str());
    lv_obj_center(label);
  }
}

void return_to_nav_page_event_cb(lv_event_t *e) {
  typedef void (*prev_cb_func)();

  lv_event_code_t code = lv_event_get_code(e);
  prev_cb_func hide_current_page = (prev_cb_func) lv_event_get_user_data(e);
  if (code == LV_EVENT_GESTURE) {
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    switch (dir) {
      case LV_DIR_RIGHT: // from left to right
        printf("Swipe from left to right: return\n");
        hide_current_page();
        drawNavPage();
        break;
      default:
        printf("Unhandled gesture\n");
        break;
    }
  }
}