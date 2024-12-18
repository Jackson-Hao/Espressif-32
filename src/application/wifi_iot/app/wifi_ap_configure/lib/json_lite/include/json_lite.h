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

jsonResolut_t* json_parse_analyzed(const char *json_str);
char* json_msg_ret(char* msg);

#endif // __JSON_LITE_H__