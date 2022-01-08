// std library

// other libraries
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// project header files
#include "utils.hpp"

void delayInTick(uint32_t ticks) {
  vTaskDelay(ticks);
}

void delayInMs(uint32_t timeMs) {
  vTaskDelay(timeMs / portTICK_PERIOD_MS);
}