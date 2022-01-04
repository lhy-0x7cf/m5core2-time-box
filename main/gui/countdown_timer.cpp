// std library
#include <cstdio>
#include <cstdlib>

// other libraries
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lvgl.h>

// project header files
#include "countdown_timer.hpp"

// common variables
struct Duration {
  // constants
  static const uint32_t kDefaultMinute = 5;
  static const uint32_t kMinuteToMs = 60000;
  static const uint32_t kDefaultSecond = 0;
  static const uint32_t kSecondToMs = 1000;

  uint32_t minute = kDefaultMinute;
  uint32_t second = kDefaultSecond;

  uint32_t toMillisecond() {
    return (minute * kMinuteToMs) + (second * kSecondToMs);
  }

} duration;
static lv_obj_t *separate_column_label;

/**
 * time picker
 */
static lv_obj_t *minute_roller;
static lv_obj_t *second_roller;
static lv_obj_t *start_btn;

static void hideTimePicker() {
  // lv_obj_add_flag(separate_column_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(minute_roller, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(second_roller, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(start_btn, LV_OBJ_FLAG_HIDDEN);
}

static void showTimePicker() {
  // lv_obj_clear_flag(separate_column_label, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(minute_roller, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(second_roller, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(start_btn, LV_OBJ_FLAG_HIDDEN);
}

static void minute_roller_event_cb(lv_event_t *e) {
  static char buf[3];
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    lv_roller_get_selected_str(obj, buf, sizeof(buf));
    duration.minute = atoi(buf);
    printf("minute=%d\n", duration.minute);
  }
}

static void second_roller_event_cb(lv_event_t *e) {
  static char buf[3];
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    lv_roller_get_selected_str(obj, buf, sizeof(buf));
    duration.second = atoi(buf);
    printf("second=%d\n", duration.second);
  }
}

static void start_btn_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    hideTimePicker();
    drawCountdownTimer();
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
  static const uint8_t kWidgetGapX = 15;
  static const uint8_t kWidgetCenterOffsetY = 35;

  // roller style
  static lv_style_t roller_style;
  lv_style_init(&roller_style);
  lv_style_set_pad_all(&roller_style, 0);
  lv_style_set_border_width(&roller_style, 0);
  lv_style_set_outline_width(&roller_style, 0);
  lv_style_set_bg_opa(&roller_style, LV_OPA_TRANSP);
  lv_style_set_text_font(&roller_style, &lv_font_montserrat_48);

  // button style
  static lv_style_t btn_style;
  lv_style_init(&btn_style);
  lv_style_set_pad_all(&btn_style, 0);
  lv_style_set_border_width(&btn_style, 0);
  lv_style_set_outline_width(&btn_style, 0);
  lv_style_set_bg_opa(&btn_style, LV_OPA_TRANSP);
  lv_style_set_text_font(&btn_style, &lv_font_montserrat_36);

  // create separate column
  separate_column_label = lv_label_create(lv_scr_act());
  lv_label_set_text(separate_column_label, ":");
  lv_obj_set_style_text_font(separate_column_label, &lv_font_montserrat_24, 0);
  lv_obj_align(separate_column_label, LV_ALIGN_CENTER, 0, -kWidgetCenterOffsetY);

  // create minute roller
  minute_roller = lv_roller_create(lv_scr_act());
  lv_obj_add_style(minute_roller, &roller_style, 0);
  lv_obj_set_width(minute_roller, kRollerWidth);
  lv_roller_set_options(minute_roller, options, LV_ROLLER_MODE_NORMAL);
  lv_roller_set_visible_row_count(minute_roller, kVisibleRowCount);
  lv_roller_set_selected(minute_roller, duration.minute, LV_ANIM_OFF);
  lv_obj_add_event_cb(minute_roller, minute_roller_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_set_style_text_align(minute_roller, LV_TEXT_ALIGN_RIGHT, 0);
  lv_obj_set_style_bg_opa(minute_roller, LV_OPA_TRANSP, LV_PART_SELECTED); // disable this when testing
  lv_obj_align_to(minute_roller, separate_column_label, LV_ALIGN_RIGHT_MID, -kWidgetGapX, 0);

  // create second roller
  second_roller = lv_roller_create(lv_scr_act());
  lv_obj_add_style(second_roller, &roller_style, 0);
  lv_obj_set_width(second_roller, kRollerWidth);
  lv_roller_set_options(second_roller, options, LV_ROLLER_MODE_NORMAL);
  lv_roller_set_visible_row_count(second_roller, kVisibleRowCount);
  lv_roller_set_selected(second_roller, duration.second, LV_ANIM_OFF);
  lv_obj_add_event_cb(second_roller, second_roller_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_set_style_text_align(second_roller, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_set_style_bg_opa(second_roller, LV_OPA_TRANSP, LV_PART_SELECTED); // disable this when testing
  lv_obj_align_to(second_roller, separate_column_label, LV_ALIGN_LEFT_MID, kWidgetGapX, 0);

  // create start button
  start_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_style(start_btn, &btn_style, 0);
  lv_obj_t *start_btn_label = lv_label_create(start_btn);
  lv_label_set_text(start_btn_label, LV_SYMBOL_PLAY);
  lv_obj_align(start_btn, LV_ALIGN_CENTER, 0, kWidgetCenterOffsetY);
  lv_obj_add_event_cb(start_btn, start_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

/**
 * countdown timer
 */
static lv_obj_t *progress_arc;
static lv_obj_t *minute_label;
static lv_obj_t *second_label;
static lv_obj_t *return_btn;

static void set_angle(void *obj, int32_t v) {
  lv_arc_set_value(static_cast<lv_obj_t *>(obj), v);
}

void drawCountdownTimer() {
  // constants
  static const uint8_t kProgressArcDiameter = 200;
  static const uint8_t kWidgetGapX = 10;

  // label style
  static lv_style_t label_style;
  lv_style_init(&label_style);
  lv_style_set_text_font(&label_style, &lv_font_montserrat_36);

  // center separate column
  lv_obj_center(separate_column_label);

  // draw minute label
  minute_label = lv_label_create(lv_scr_act());
  lv_obj_add_style(minute_label, &label_style, 0);
  lv_label_set_text_fmt(minute_label, "%02d", duration.minute);
  lv_obj_align_to(minute_label, separate_column_label, LV_ALIGN_RIGHT_MID, -kWidgetGapX, 0);

  // draw second label
  second_label = lv_label_create(lv_scr_act());
  lv_obj_add_style(second_label, &label_style, 0);
  lv_label_set_text_fmt(second_label, "%02d", duration.second);
  lv_obj_align_to(second_label, separate_column_label, LV_ALIGN_LEFT_MID, kWidgetGapX, 0);

  // draw progress arc
  progress_arc = lv_arc_create(lv_scr_act());
  lv_arc_set_rotation(progress_arc, 270);
  lv_arc_set_bg_angles(progress_arc, 0, 360);
  lv_obj_set_size(progress_arc, kProgressArcDiameter, kProgressArcDiameter);
  lv_obj_remove_style(progress_arc, NULL, LV_PART_KNOB);
  lv_obj_center(progress_arc);

  // set the animation of progress arc
  lv_anim_t anim;
  lv_anim_init(&anim);
  lv_anim_set_var(&anim, progress_arc);
  lv_anim_set_exec_cb(&anim, set_angle);
  lv_anim_set_time(&anim, duration.toMillisecond());
  lv_anim_set_repeat_count(&anim, 0);
  // lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE); // just for the demo
  // lv_anim_set_repeat_delay(&anim, 500);
  lv_anim_set_values(&anim, 0, 100);
  lv_anim_start(&anim);
}