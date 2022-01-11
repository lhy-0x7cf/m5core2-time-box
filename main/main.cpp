// std library

// other libraries
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <nvs_flash.h>

// project header files
#include "gui/lvgl_manager.hpp"
#include "power_manager.hpp"
#include "wifi_manager.hpp"

void init_nvs() {
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
}

extern "C" void app_main(void) {
  init_nvs();
  PowerManager::instance().setup();
  LvglManager::instance().start();
  WifiManager::instance().init_sta();
}
