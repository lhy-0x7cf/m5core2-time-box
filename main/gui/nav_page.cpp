// std library
#include <string>
#include <vector>
#include <utility>

// other libraries
#include <lvgl.h>

// project header files
#include "nav_page.hpp"
#include "countdown_timer.hpp"

// constants
// static const char* const kButtonNames[] = {
//   "Timer",
//   "Local Info",
//   LV_SYMBOL_SETTINGS " Settings",
// };
// static const uint16_t kButtonNumber = sizeof(kButtonNames) / sizeof(kButtonNames[0]);

// GUI objects
static lv_obj_t *menu;
static std::vector<lv_obj_t *> buttons;

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

static void settings_btn_even_cb(lv_event_t *e) {
  // TODO: implement this
}

static std::vector<std::pair<std::string, lv_event_cb_t>> button_utils {
  {"Timer", timer_btn_even_cb},
  {"Local Info", local_info_btn_even_cb},
  {LV_SYMBOL_SETTINGS " Settings", settings_btn_even_cb}
};
static const uint16_t kButtonNumber = button_utils.size();

void showNavPage() {
  lv_obj_clear_flag(menu, LV_OBJ_FLAG_HIDDEN);
  // for (int i = 0; i < kButtonNumber; ++i) {
    // lv_obj_clear_flag(buttons[i], LV_OBJ_FLAG_HIDDEN);
  // }
}

void drawNavPage() {
  static bool is_drawn = false;
  if (is_drawn) {
    showNavPage();
    return;
  }
  is_drawn = true;

  // constants
  const uint16_t kMenuWidth = LV_HOR_RES_MAX;
  const uint16_t kMenuHeight = LV_VER_RES_MAX - 20;
  const uint16_t kColumnNumber = 2;
  const uint16_t kButtonHeight = 90;

  // menu style
  static lv_style_t menu_style;
  lv_style_set_border_width(&menu_style, 0);
  lv_style_set_outline_width(&menu_style, 0);
  lv_style_set_bg_opa(&menu_style, LV_OPA_TRANSP);

  // button style
  static lv_style_t button_style;
  lv_style_set_border_color(&button_style, lv_color_white());
  lv_style_set_border_width(&button_style, 1);
  lv_style_set_outline_width(&button_style, 0);
  lv_style_set_bg_opa(&button_style, LV_OPA_TRANSP);

  // draw the menu
  menu = lv_obj_create(lv_scr_act());
  lv_obj_set_size(menu, kMenuWidth, kMenuHeight);
  lv_obj_add_style(menu, &menu_style, 0);
  lv_obj_align(menu, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_flex_flow(menu, LV_FLEX_FLOW_ROW_WRAP);
  lv_obj_set_scrollbar_mode(menu, LV_SCROLLBAR_MODE_ACTIVE);

  // draw buttons
  buttons.resize(kButtonNumber);
  lv_obj_t *label;
  for (int i = 0; i < kButtonNumber; ++i) {
    buttons[i] = lv_btn_create(menu);
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

void hideNavPage() {
  lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
  // for (int i = 0; i < kButtonNumber; ++i) {
    // lv_obj_add_flag(buttons[i], LV_OBJ_FLAG_HIDDEN);
  // }
}