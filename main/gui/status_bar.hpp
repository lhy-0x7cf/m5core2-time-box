#ifndef M5CORE2_TIME_BOX_GUI_STATUS_DISPLAY_BAR_HPP_
#define M5CORE2_TIME_BOX_GUI_STATUS_DISPLAY_BAR_HPP_

// std library

// other libraries
#include <lvgl.h>

// project header files

// constants
static const lv_font_t status_bar_font = lv_font_montserrat_18;
static const uint16_t kStatusBarWidth = LV_HOR_RES_MAX;
static const uint16_t kStatusBarHeight = 20; // same as font size

void drawStatusBar();

#endif // M5CORE2_TIME_BOX_GUI_STATUS_DISPLAY_BAR_HPP_