#include "main.h"
#include "netNVS.h"
#include "osThread.h"
#include "ap_wifi_config.h"
#include "http_server_lite.h"

SemaphoreHandle_t sem = NULL;

static void IoTWifiInitEntry(void *arg) {
    int ret;
    int global_conf_status = 0;
    NVS_init();
    if((ret = ifWifiConfigExists()) == ESP_OK) {
        printf("wifi config exists\n");
        nvs_wifi_config_t wifi_config = NVS_read_wifi_config();
        printf("ssid: %s\n", wifi_config.ssid);
        printf("password: %s\n", wifi_config.password);
        wifi_init_sta(wifi_config.ssid, wifi_config.password);
        xSemaphoreGive(sem);
    } else {
        printf("wifi config does not exist\n");
        wifi_init_softap();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        httpd_server_start();
    }
    vTaskDelete(NULL);
}

static void IoTOtherTaskEntry(void *arg) {
    while(1) {
        printf("IoTOtherTaskEntry\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void IoTMainTaskEntry(void) {
    osThreadAttr_t attr;

    sem = xSemaphoreCreateBinary();

    attr.name = "IoTWifiInitTask";
    attr.stack_size = 8 * 1024;
    attr.priority = 5;
    attr.cpu_id = 1;

    if((osThreadCreate(&attr, IoTWifiInitEntry, NULL)) != 0) {
        printf("create IoTWifiInitTask failed\n");
    }

    xSemaphoreTake(sem, portMAX_DELAY);

    attr.name = "IoTOtherTask";
    attr.stack_size = 4 * 1024;
    attr.priority = 5;
    attr.cpu_id = 1;
    if((osThreadCreate(&attr, IoTOtherTaskEntry, NULL)) != 0) {
        printf("create IoTOtherTask failed\n");
    } 
}

APP_SERVICE_INIT(IoTMainTaskEntry);