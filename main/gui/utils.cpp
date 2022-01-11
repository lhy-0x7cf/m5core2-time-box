// std library

// other libraries
#include <lvgl.h>

// project header files
#include "utils.hpp"

void enablePressLock(lv_obj_t *obj) {
  lv_obj_add_flag(obj, LV_OBJ_FLAG_PRESS_LOCK);
}

void disablePressLock(lv_obj_t *obj) {
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_PRESS_LOCK);
}