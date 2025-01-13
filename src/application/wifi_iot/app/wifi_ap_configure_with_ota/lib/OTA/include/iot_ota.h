#ifndef IOT_OTA_H
#define IOT_OTA_H

#include "mqtt_config.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "json_lite.h"
#include "esp_log.h"
#include "esp_err.h"

#define DEVICE_ID MQTT_BROKER_USER
#define IOT_OC_MQTT_PROFILE_OTA_TOPICFMT "$oc/devices/%s/sys/events/down"
#define IOT_OC_MQTT_PROFILE_OTA_TOPICFMT_UP "$oc/devices/%s/sys/events/up"

typedef struct {
    int status;
    char *version;
} UpdateStatus;

typedef void (*ota_event_finish_callback)(UpdateStatus *status);


char* OTA_get_current_version(void);
uint8_t OTA_if_ota_topic(char* topic);
esp_err_t OTA_info_config(const char* url, const char* version, const char* token, ota_event_finish_callback callback);
char* generate_ota_status(const char* object_device_id, int result_code, const char* version);
esp_err_t OTA_start(void);
char* OTA_get_current_version(void);


#endif