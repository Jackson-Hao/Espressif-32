#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "iot_cloud_oc.h"
#include "iot_wifi_sta.h"
#include "iot_gpio_config.h"

#define ATTR_NAME "IoTPropertyReportTask"
#define ATTR_TASK_STACK_SIZE 1024*4
#define ATTR_TASK_PRIORITY 5


static void IotMainTaskEntry(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    
    int status = (int)wifi_sta_init(NULL);
    if(status != ESP_OK) {
        printf("wifi_sta_init failed\n");
        return;
    }

    IoTCloudServiceTaskEntry(MQTT_BROKER_URL, MQTT_CLIENT_ID, MQTT_BROKER_USER, MQTT_BROKER_PASS);
    xTaskCreatePinnedToCore(IoTPropertyReportTask, ATTR_NAME, ATTR_TASK_STACK_SIZE, NULL, ATTR_TASK_PRIORITY, NULL, 1);
    xTaskCreatePinnedToCore(IoTGpioServiceInit, "IoTGpioServiceInit", 1024*4, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(IoTGpioLedMainTask, "IoTGpioMainTask", 1024*2, NULL, 5, NULL, 1);
    // xTaskCreatePinnedToCore(IoTGpioDHTMainTask, "IoTGpioDHTMainTask", 1024*4, NULL, 5, NULL, 1);
}

APP_SERVICE_INIT(IotMainTaskEntry)