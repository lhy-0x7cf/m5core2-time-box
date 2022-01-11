// std library
#include <thread>

// other libraries
#include <lvgl.h>
#include <lvgl_helpers.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// project header files
#include "lvgl_manager.hpp"
#include "nav_page.hpp"
#include "sdkconfig.h"
#include "status_bar.hpp"
#include "../utils.hpp"

// global variables
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color16_t color_buf[2][DISP_BUF_SIZE];
SemaphoreHandle_t xGuiSemaphore;

// constants
static const uint32_t kLvTickPeriodMs = 1;

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
  drawStatusBar();
  // NOTE:
  // This delay is a must!
  // Without this delay, a small crash will happen and the battery percentage cannot be displayed.
  delayInTick(1);
  drawNavPage();

  // infinite loop
  while (1) {
    delayInTick(1);

    // try to take the semaphore, call lvgl related function on success
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
      lv_task_handler();
      xSemaphoreGive(xGuiSemaphore);
    }
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
  xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 0, NULL, 1);
}