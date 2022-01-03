// std library
#include <cstdio>

// other libraries
#include <lvgl.h>

// project header files
#include "countdown_timer.hpp"

static void minute_roller_event_cb(lv_event_t *e) {
  static char buf[3];
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    lv_roller_get_selected_str(obj, buf, sizeof(buf));
    printf("minute=%s\n", buf);
  }
}

static void second_roller_event_cb(lv_event_t *e) {
  static char buf[3];
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    lv_roller_get_selected_str(obj, buf, sizeof(buf));
    printf("second=%s\n", buf);
  }
}

void drawTimePicker() {
  // constants
  static const char *options = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n"
                               "10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n"
                               "20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n"
                               "30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n"
                               "40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n"
                               "50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
  static const uint8_t kVisibleRowCount = 1;
  static const uint8_t kRollerWidth = 100;

  // roller style
  static lv_style_t roller_style;
  lv_style_init(&roller_style);
  lv_style_set_border_width(&roller_style, 0);
  lv_style_set_outline_width(&roller_style, 0);
  lv_style_set_bg_opa(&roller_style, LV_OPA_TRANSP);
  lv_style_set_pad_all(&roller_style, 0);
  // lv_style_set_text_font(&roller_style, LV_FONT_MONTSERRAT_36);

  // create separate column
  lv_obj_t *separate_column_label = lv_label_create(lv_scr_act());
  lv_label_set_text(separate_column_label, ":");
  lv_obj_center(separate_column_label);

  // create minute roller
  lv_obj_t *minute_roller = lv_roller_create(lv_scr_act());
  lv_obj_set_width(minute_roller, kRollerWidth);
  lv_roller_set_options(minute_roller, options, LV_ROLLER_MODE_NORMAL);
  lv_roller_set_visible_row_count(minute_roller, kVisibleRowCount);
  lv_roller_set_selected(minute_roller, 5, LV_ANIM_OFF);
  lv_obj_add_event_cb(minute_roller, minute_roller_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_style(minute_roller, &roller_style, 0);
  lv_obj_set_style_text_align(minute_roller, LV_TEXT_ALIGN_RIGHT, 0);
  // lv_obj_set_style_bg_opa(minute_roller, LV_OPA_TRANSP, LV_PART_SELECTED); // disable this when testing
  lv_obj_align_to(minute_roller, separate_column_label, LV_ALIGN_RIGHT_MID, -10, 0);
  
  // create second roller
  lv_obj_t *second_roller = lv_roller_create(lv_scr_act());
  lv_obj_set_width(second_roller, kRollerWidth);
  lv_roller_set_options(second_roller, options, LV_ROLLER_MODE_NORMAL);
  lv_roller_set_visible_row_count(second_roller, kVisibleRowCount);
  lv_roller_set_selected(second_roller, 0, LV_ANIM_OFF);
  lv_obj_add_event_cb(second_roller, second_roller_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_style(second_roller, &roller_style, 0);
  lv_obj_set_style_text_align(second_roller, LV_TEXT_ALIGN_LEFT, 0);
  // lv_obj_set_style_bg_opa(second_roller, LV_OPA_TRANSP, LV_PART_SELECTED); // disable this when testing
  lv_obj_align_to(second_roller, separate_column_label, LV_ALIGN_LEFT_MID, 10, 0);
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