#include <stdio.h>
#include "main.h"
#include "osThread.h"

static void IoTEchoTaskEntry1(void* arg) {
    while (1) {
        printf("IoTEchoTaskEntry1\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void IoTEchoTaskEntry2(void* arg) {
    while (1) {
        printf("IoTEchoTaskEntry2\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void IoTMainTaskEntry(void) {
    int ret = 0;
    osThreadAttr_t attr;

    attr.name = "IoTEchoTask1";
    attr.stack_size = 1024*4;
    attr.priority = 5;
    attr.cpu_id = 1;
    if((ret = osThreadCreate(&attr, IoTEchoTaskEntry1, NULL)) != 0) {
        printf("IoTEchoTask create failed\n");
    }

    attr.name = "IoTEchoTask2";
    attr.stack_size = 1024*4;
    attr.priority = 5;
    attr.cpu_id = 1;
    if((ret = osThreadCreate(&attr, IoTEchoTaskEntry2, NULL)) != 0) {
        printf("IoTEchoTask create failed\n");
    }
}

APP_SERVICE_INIT(IoTMainTaskEntry)