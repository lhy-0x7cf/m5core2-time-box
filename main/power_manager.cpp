// std library
#include <algorithm>
#include <stdint.h>

// other libraries
#include <axp192.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2c_manager.h>
#include <sdkconfig.h>

// project header files
#include "power_manager.hpp"

#define I2C_0 static_cast<const axp192_t *>(i2c_hal(0))

void PowerManager::setup() {
  axp192_init(I2C_0);
  axp192_ioctl(I2C_0, AXP192_GPIO0_SET_LEVEL, AXP192_HIGH);  // M-Bus Power
  axp192_ioctl(I2C_0, AXP192_GPIO1_SET_LEVEL, AXP192_LOW);   // Enable LED
  axp192_ioctl(I2C_0, AXP192_GPIO2_SET_LEVEL, AXP192_LOW);   // Disable speaker
  axp192_ioctl(I2C_0, AXP192_LDO2_SET_VOLTAGE, 3300);        // Set LDO2 LCD&TP voltage
  axp192_ioctl(I2C_0, AXP192_LDO2_ENABLE);                   // Enable LDO2
  axp192_ioctl(I2C_0, AXP192_GPIO4_SET_LEVEL, AXP192_LOW);   // LCD&TP Reset
  vTaskDelay(100 / portTICK_PERIOD_MS);
  axp192_ioctl(I2C_0, AXP192_GPIO4_SET_LEVEL, AXP192_HIGH);  // LCD&TP Hold
  vTaskDelay(100 / portTICK_PERIOD_MS);
}

uint32_t PowerManager::getBatteryLevel() {
  // TODO: change printf to esp logging API
  float battery_voltage = -1.0;
  int rc = axp192_read(I2C_0, AXP192_BATTERY_VOLTAGE, &battery_voltage);
  if (rc == AXP192_OK) {
    // Converting method copied from https://github.com/m5stack/M5Core2/blob/master/src/AXP192.cpp#L269
    // printf("battery_voltage=%0.2f\n", battery_voltage);
    float battery_percentage = (battery_voltage < 3.248088) ? 0 : (battery_voltage - 3.120712) * 100;
    // printf("battery_percentage=%0.2f\n", battery_percentage);
    return static_cast<uint32_t>(std::min(battery_percentage, 100.0f));
  }
  printf("Failed to read the battery voltage.");
  return 0;
}

#undef I2C_0