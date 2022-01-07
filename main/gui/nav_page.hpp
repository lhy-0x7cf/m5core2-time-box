#ifndef M5CORE2_TIME_BOX_GUI_NAV_PAGE_HPP_
#define M5CORE2_TIME_BOX_GUI_NAV_PAGE_HPP_

// std library

// other libraries
#include <lvgl.h>

// project header files

/**
 * @brief If the nav page is drawn once, this function will not draw it again.
 *        Instead, it will just show the GUI objects by removing LV_OBJ_FLAG_HIDDEN.
 */
void drawNavPage();

void return_to_nav_page_event_cb(lv_event_t *e);

#endif // M5CORE2_TIME_BOX_GUI_NAV_PAGE_HPP_