// std library

// other libraries
#include <lvgl.h>

// project header files
#include "countdown_timer.hpp"

void drawTimePicker() {

}

static void set_angle(void *obj, int32_t v) {
  lv_arc_set_value(static_cast<lv_obj_t *>(obj), v);
}

void drawCountdownTimer() {
  // draw progress arc
  lv_obj_t *progress_arc = lv_arc_create(lv_scr_act());
  lv_arc_set_rotation(progress_arc, 270);
  lv_arc_set_bg_angles(progress_arc, 0, 360);
  lv_obj_remove_style(progress_arc, NULL, LV_PART_KNOB);
  lv_obj_center(progress_arc);

  // set the animation
  lv_anim_t anim;
  lv_anim_init(&anim);
  lv_anim_set_var(&anim, progress_arc);
  lv_anim_set_exec_cb(&anim, set_angle);
  lv_anim_set_time(&anim, 1000);
  lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
  lv_anim_set_repeat_delay(&anim, 500);
  lv_anim_set_values(&anim, 0, 100);
  lv_anim_start(&anim);
}