#ifndef M5CORE2_TIME_BOX_GUI_UTILS_HPP_
#define M5CORE2_TIME_BOX_GUI_UTILS_HPP_

// std library

// other libraries
#include <lvgl.h>

// project header files

/**
 * NOTE:
 * Calling enablePressLock(lv_obj_t *obj) on background objects (e.g., lv_scr_act())
 * and calling disablePressLock(lv_obj_t *obj) on foreground objects. This will make
 * sure that the gesture event for background objects will not cause the click event
 * for foreground objects
 */
void enablePressLock(lv_obj_t *obj);

void disablePressLock(lv_obj_t *obj);

#endif // M5CORE2_TIME_BOX_GUI_UTILS_HPP_