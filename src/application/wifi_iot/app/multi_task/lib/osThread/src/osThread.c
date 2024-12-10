
#include "osThread.h"

int osThreadCreate(osThreadAttr_t *attr, void (*entry)(void *), void *arg) {
    TaskHandle_t task;
    BaseType_t ret = xTaskCreatePinnedToCore(entry, attr->name, attr->stack_size, arg, attr->priority, &task, attr->cpu_id);
    return ret == pdPASS ? 0 : -1;
}