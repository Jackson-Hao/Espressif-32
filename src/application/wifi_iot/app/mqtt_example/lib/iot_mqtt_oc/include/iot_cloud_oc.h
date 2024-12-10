#ifndef __IOT_CLOUD_OC_H__
#define __IOT_CLOUD_OC_H__
#include "mqtt_config.h"


void IoTCloudServiceTaskEntry(const char *mqtt_url, const char *client_id, const char *mqtt_user, const char *mqtt_pass);
void IoTPropertyReportTask(void *pvParameters);
void IoTGpioLedMainTask(void *pvParameters);
void IoTGpioDHTMainTask(void* pvParameters);

#endif

