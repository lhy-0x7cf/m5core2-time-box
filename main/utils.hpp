#ifndef M5CORE2_TIME_BOX_UTILS_HPP_
#define M5CORE2_TIME_BOX_UTILS_HPP_
// std library

// other libraries
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// project header files

void delayInTick(uint32_t ticks);

void delayInMs(uint32_t timeMs);

#endif // M5CORE2_TIME_BOX_UTILS_HPP_