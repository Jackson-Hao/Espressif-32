#ifndef __NET_NVS_H__
#define __NET_NVS_H__

#include "esp_err.h"

typedef struct {
    char* ssid;
    char* password;
} nvs_wifi_config_t;

void NVS_init();
void NVS_save_wifi_config(char *ssid, char *password);
nvs_wifi_config_t NVS_read_wifi_config(void);
esp_err_t ifWifiConfigExists(void);

#define NVS_NAMESPACE "WIFI_CONFIG"

#endif