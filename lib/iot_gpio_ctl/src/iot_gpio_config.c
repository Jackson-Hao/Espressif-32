#include "iot_gpio_config.h"

#define TAG "[GPIO Service]:"



void IoTGpioServiceInit(void *pvParameters) {
    gpio_config_t gpio_conf_led;
    gpio_conf_led.pin_bit_mask = GPIO_LED_PIN;
    gpio_conf_led.mode = GPIO_MODE_OUTPUT;
    gpio_conf_led.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf_led.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_conf_led.intr_type = GPIO_INTR_DISABLE;

    gpio_config_t gpio_conf_dht11;
    gpio_conf_dht11.pin_bit_mask = 1ULL << DHT11_PIN;
    gpio_conf_dht11.mode = GPIO_MODE_INPUT;
    gpio_conf_dht11.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_conf_dht11.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_conf_dht11.intr_type = GPIO_INTR_DISABLE;

    if(gpio_config(&gpio_conf_led) != ESP_OK) {
        ESP_LOGI(TAG, "GPIO init failed\n");
        return;
    } else {
        ESP_LOGI(TAG, "GPIO init success\n");
    }

    if(gpio_config(&gpio_conf_dht11) != ESP_OK) {
        ESP_LOGI(TAG, "GPIO init failed\n");
        return;
    } else {
        ESP_LOGI(TAG, "GPIO init success\n");
    }

    gpio_set_level(GPIO_LED_IO, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LED_IO, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    vTaskDelete(NULL);
}

