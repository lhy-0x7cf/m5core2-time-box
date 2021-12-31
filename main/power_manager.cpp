// std library
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
  // check the result with AXP192_OK
  uint32_t voltage_read = -1;
  int rc = axp192_read(I2C_0, AXP192_BATTERY_VOLTAGE, &voltage_read);
  if (rc == AXP192_OK) {
    static const float kAdcLsb = 1.1 / 1000.0;
    float voltage = voltage_read * kAdcLsb;
    float battery_percentage = (voltage < 3.248088) ? (0) : (voltage - 3.120712) * 100;
    return static_cast<uint32_t>(battery_percentage);
  }
  return 0;
}

#undef I2C_0