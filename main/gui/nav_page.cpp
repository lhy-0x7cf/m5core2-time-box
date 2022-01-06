// std library

// other libraries
#include <lvgl.h>

// project header files
#include "nav_page.hpp"

// constants
static const char* const kButtonNames[] = {
  /*MY_SYMBOL_LOCAL*/ "Local Info",
  /*MY_SYMBOL_CLOCK*/ "Countdown Timer",
  LV_SYMBOL_SETTINGS " Settings"
};
static const uint16_t kButtonNumber = sizeof(kButtonNames) / sizeof(kButtonNames[0]);

// GUI objects
static lv_obj_t *menu;
static std::vector<lv_obj_t *> buttons;

void showNavPage() {
  for (int i = 0; i < kButtonNumber; ++i) {
    lv_obj_clear_flag(buttons[i], LV_OBJ_FLAG_HIDDEN);
  }
}

void drawNavPage() {
  static bool is_drawn = false;
  if (is_drawn) {
    showNavPage();
    return;
  }
  is_drawn = true;

  // constants
  const uint16_t kMenuWidth = LV_HOR_RES_MAX - 60;
  const uint16_t kMenuHeight = LV_VER_RES_MAX - 40;
  const uint16_t kButtonHeight = 80;

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
  lv_obj_center(menu);
  lv_obj_set_flex_flow(menu, LV_FLEX_FLOW_ROW_WRAP);
  lv_obj_set_scrollbar_mode(menu, LV_SCROLLBAR_MODE_ACTIVE);

  // draw buttons
  buttons.resize(kButtonNumber);
  lv_obj_t *label;
  for (int i = 0; i < kButtonNumber; ++i) {
    buttons[i] = lv_btn_create(menu);
    lv_obj_set_size(buttons[i], LV_PCT(100), kButtonHeight);
    lv_obj_add_style(buttons[i], &button_style, 0);
    label = lv_label_create(buttons[i]);
    lv_label_set_text(label, kButtonNames[i]);
    lv_obj_center(label);
  }
}

void hideNavPage() {
  // TODO: try to hide the menu to see whether flag hidden will be applied to child elements automatically or not
  for (int i = 0; i < kButtonNumber; ++i) {
    lv_obj_add_flag(buttons[i], LV_OBJ_FLAG_HIDDEN);
  }
}