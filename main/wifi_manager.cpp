// std library

// other libraries
#include <esp_wifi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// project header files
#include "wifi_manager.hpp"

void WifiManager::init_sta() {
  // TODO: fully implement this
  ESP_ERROR_CHECK(esp_netif_init());
  esp_netif_create_default_wifi_sta();
  wifi_init_config_t wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
  printf("init_config size=%d\n", sizeof(wifi_init_config_t));
  esp_err_t err = esp_wifi_init(&wifi_init_cfg);
  printf("%s\n", esp_err_to_name(err));
}