#ifndef __JSON_LITE_H__
#define __JSON_LITE_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>
#include "esp_log.h"

typedef struct {
    char* ssid;
    char* password;
    char* client_ip;
} jsonResolut_t;

typedef struct {
    char *version;
    char *url;
    char *token;
} FirmwareInfo;

jsonResolut_t* json_parse_analyzed(const char *json_str);
char* json_msg_ret(char* msg);
char* extract_event_type(const char *json_str);
FirmwareInfo* extract_firmware_info(const char *json_str);

#endif // __JSON_LITE_H__