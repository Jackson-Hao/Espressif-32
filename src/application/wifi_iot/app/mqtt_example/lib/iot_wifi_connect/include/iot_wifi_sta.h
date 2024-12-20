#ifndef _WIFI_MANAGER_H_
#define _WIFI_MANAGER_H_
#include "esp_err.h"

#define DEFAULT_WIFI_SSID           "HUAWEI"
#define DEFAULT_WIFI_PASSWORD       "haozihan2004"

typedef enum {
    WIFI_DISCONNECTED,
    WIFI_CONNECTED,
} WIFI_EV_e;

typedef void(*wifi_event_cb)(WIFI_EV_e);

esp_err_t wifi_sta_init(wifi_event_cb f);


#endif
