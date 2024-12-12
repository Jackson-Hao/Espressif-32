#ifndef __OSTHREAD_H__
#define __OSTHREAD_H__

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct {
    char *name;
    uint32_t stack_size;
    uint8_t priority;
    uint8_t cpu_id;
}osThreadAttr_t;

int osThreadCreate(osThreadAttr_t *attr, void (*entry)(void *), void *arg);
void osThreadTerminate(void);


#endif