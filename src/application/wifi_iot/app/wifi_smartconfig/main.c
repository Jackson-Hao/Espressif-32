#include <stdio.h>
#include "main.h"
#include "osThread.h"
#include "iot_wifi_config.h"

static void IoTWifiTaskEntry(void *arg) {
    wifi_config_init();
}

static void IoTMainTaskEntry(void) {
    osThreadAttr_t attr;

    attr.name = "IoTWifiTask";
    attr.stack_size = 4 * 1024;
    attr.priority = 5;
    attr.cpu_id = 1;

    if((osThreadCreate(&attr, IoTWifiTaskEntry, NULL)) != 0) {
        printf("create IoTWifiTask failed\n");
    }
}

APP_SERVICE_INIT(IoTMainTaskEntry);