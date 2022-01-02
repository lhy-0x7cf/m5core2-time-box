// std library
#include <chrono>
#include <thread>

// other libraries
#include <lvgl.h>
#include <lvgl_helpers.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

// project header files
#include "lvgl_manager.hpp"
#include "power_manager.hpp"
#include "sdkconfig.h"

// global variables
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color16_t color_buf[2][DISP_BUF_SIZE];
SemaphoreHandle_t xGuiSemaphore;

// constants
static const uint32_t kLvTickPeriodMs = 1;

void drawBatteryPercentage() {
  // battery percentage text
  static uint32_t battery_percentage;
  static lv_obj_t *battery_percentage_text = lv_label_create(lv_scr_act());
  lv_obj_align(battery_percentage_text, LV_ALIGN_TOP_RIGHT, -5, 5);
  while (true) {
    battery_percentage = PowerManager::instance().getBatteryLevel();
    printf("[%s] battery_percentage=%d\n", __FUNCTION__, battery_percentage);
    lv_label_set_text_fmt(battery_percentage_text, "%d%%", battery_percentage);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

static void set_angle(void *obj, int32_t v) {
  lv_arc_set_value(static_cast<lv_obj_t *>(obj), v);
}

void drawCountDownTimer() {
  // draw progress arc
  lv_obj_t *progress_arc = lv_arc_create(lv_scr_act());
  lv_arc_set_rotation(progress_arc, 270);
  lv_arc_set_bg_angles(progress_arc, 0, 360);
  lv_obj_remove_style(progress_arc, NULL, LV_PART_KNOB);
  lv_obj_center(progress_arc);

  lv_anim_t anim;
  lv_anim_init(&anim);
  lv_anim_set_var(&anim, progress_arc);
  lv_anim_set_exec_cb(&anim, set_angle);
  lv_anim_set_time(&anim, 1000);
  lv_anim_set_repeat_count(&anim, 1);    /*Just for the demo*/
  lv_anim_set_repeat_delay(&anim, 500);
  lv_anim_set_values(&anim, 0, 100);
  lv_anim_start(&anim);
}

static void lv_tick_task(void *args) {
  (void) args;
  lv_tick_inc(kLvTickPeriodMs);
}


void guiTask(void *pvParameter) {
  (void) pvParameter;
  xGuiSemaphore = xSemaphoreCreateMutex();

  // create and start anim timer for lv_tick_inc
  const esp_timer_create_args_t periodic_timer_args = {
    .callback = &lv_tick_task,
    .name = "periodic_gui"
  };
  esp_timer_handle_t periodic_timer;
  ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, kLvTickPeriodMs * 1000));

  // gui code
  std::thread battery_status_thread(drawBatteryPercentage);
  drawCountDownTimer();
  
  // forever loop
  while (1) {
    // delay 1 tick (assumes FreeRTOS tick is 10ms)
    vTaskDelay(pdMS_TO_TICKS(10));

    // try to take the semaphore, call lvgl related function on success
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
      lv_task_handler();
      xSemaphoreGive(xGuiSemaphore);
    }
  }

  // join the thread
  if (battery_status_thread.joinable()) {
    battery_status_thread.join();
  }
}

void LvglManager::start() {
  // init lvgl and its driver
  lv_init();
  lvgl_driver_init();

  // init display
  lv_disp_draw_buf_init(&draw_buf, &color_buf[0], &color_buf[1], DISP_BUF_SIZE);

  // init display driver
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = LV_HOR_RES_MAX;
  disp_drv.ver_res = LV_VER_RES_MAX;
  disp_drv.flush_cb = disp_driver_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // init input device
#if CONFIG_LV_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
  lv_indev_drv_init(&indev_drv);
	indev_drv.read_cb = touch_driver_read;
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	lv_indev_drv_register(&indev_drv);
#else
#error "Touch Controller Not Found"
#endif

  // NOTE:
  // If you want to use anim task to create the graphic, you NEED to create anim Pinned task.
  // Otherwise there can be problem such as memory corruption and so on.
  // When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0.
  // If the taks is pinned to core 1, the CPU usage will be 30% higher than core 0.
  xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 0, NULL, 0);
}