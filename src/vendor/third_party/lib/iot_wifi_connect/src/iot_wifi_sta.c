#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "iot_wifi_sta.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

static const char *TAG = "[WIFI_TASK]:";

static SemaphoreHandle_t s_wifi_connect_sem = NULL;

static wifi_event_cb    wifi_cb = NULL;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data) {   
    if(event_base == WIFI_EVENT) {
        switch (event_id) {
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_CONNECTED: 
            ESP_LOGI(TAG, "connected to AP");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            ESP_LOGI(TAG,"connect to the AP fail,retry now");
            break;
        default:
            break;
        }
    }
    if(event_base == IP_EVENT) {
        switch(event_id) {
            case IP_EVENT_STA_GOT_IP:
                if(wifi_cb)
                    wifi_cb(WIFI_CONNECTED);
                ESP_LOGI(TAG,"get ip address");
                xSemaphoreGive(s_wifi_connect_sem);
                break;
        }
    }
}

esp_err_t wifi_sta_init(wifi_event_cb f) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();


    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    s_wifi_connect_sem = xSemaphoreCreateBinary();

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&wifi_event_handler,NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&wifi_event_handler,NULL));


    wifi_config_t wifi_config = { 
        .sta = { 
            .ssid = DEFAULT_WIFI_SSID,
            .password = DEFAULT_WIFI_PASSWORD,
	        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    wifi_cb = f;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    xSemaphoreTake(s_wifi_connect_sem, portMAX_DELAY);

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    return ESP_OK;
}