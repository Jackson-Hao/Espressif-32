#ifndef __IOT_GPIO_CONFIG_H__
#define __IOT_GPIO_CONFIG_H__

#include "sdkconfig.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

#define GPIO_LED_IO 5
#define GPIO_LED_PIN 1ULL<<GPIO_LED_IO  
#define DHT11_PIN 6

typedef struct{
    uint8_t gpio_pin;
    uint8_t gpio_status;
} gpio_ctl_t;

void IoTGpioServiceInit(void *pvParameters);

#endif