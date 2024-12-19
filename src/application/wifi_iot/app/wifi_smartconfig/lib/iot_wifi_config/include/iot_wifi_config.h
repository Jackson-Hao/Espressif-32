#ifndef __IOT_WIFI_CONFIG_H__
#define __IOT_WIFI_CONFIG_H__

#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"

esp_err_t wifi_config_init(void);

#endif