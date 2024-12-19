#include <string.h>
#include "ap_wifi_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "http_server_lite.h"
#include "ping/ping_sock.h"
#include "lwip/err.h"
#include "lwip/sys.h"

static const char *TAG_AP = "[WIFI_AP_TASK]:";
static const char *TAG_STA = "[WIFI_STA_TASK]:";

SemaphoreHandle_t sem1 = NULL;

static char* get_mac_address(void) {
    uint8_t *mac = (uint8_t *)malloc(6);
    char* mac_str = (char *)malloc(18);
    esp_efuse_mac_get_default(mac);
    sprintf(mac_str, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    printf("MAC address: %s\n", mac_str);
    return mac_str;
}

static void wifi_event_ap_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data) {
    switch(event_id){
        case WIFI_EVENT_AP_STACONNECTED:{
            wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
            ESP_LOGI(TAG_AP, "station "MACSTR" join, AID=%d", MAC2STR(event->mac), event->aid);
            break;
        }
        case WIFI_EVENT_AP_STADISCONNECTED:{
            wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
            ESP_LOGI(TAG_AP, "station "MACSTR" leave, AID=%d", MAC2STR(event->mac), event->aid);
            break;
        }
        default: break;
    }
}

void wifi_init_softap(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_ap_handler, NULL, NULL));
    char* ssid = strcat(AP_WIFI_SSID, (char*)get_mac_address());
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "",
            .ssid_len = strlen(ssid),
            .channel = AP_WIFI_CHANNEL,
            .max_connection = AP_WIFI_MAX_STA_CONN,
            .authmode = WIFI_AUTH_OPEN,
            .pmf_cfg = {
                    .required = false,
            },
        },
    };

    strncpy((char*)wifi_config.ap.ssid, ssid, sizeof(wifi_config.ap.ssid));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    ESP_LOGI(TAG_AP, "wifi_init_softap finished. SSID:%s channel:%d",AP_WIFI_SSID, AP_WIFI_CHANNEL);
}

static void wifi_event_sta_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data) {
    int connect_time = 0;
    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_START ||event_id == WIFI_EVENT_STA_DISCONNECTED)) {
        if(connect_time < 5) {
            ESP_LOGI(TAG_STA, "Begin to connect the AP");
            esp_wifi_connect();
            connect_time++;
        } else {
            ESP_LOGE(TAG_STA, "Connect the AP failed");
            esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_sta_handler);
            ESP_LOGI(TAG_STA, "Unregister the event handler");
        }
        
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG_STA, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        xSemaphoreGive(sem1);
    }
}

void wifi_init_sta(char* ssid, char* password) {

    sem1 = xSemaphoreCreateBinary();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_sta_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_sta_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = "",
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    sem1 = xSemaphoreCreateBinary();

    ESP_LOGI(TAG_STA, "wifi_init_sta finished.");
    ESP_LOGI(TAG_STA, "SSID:%s, password:%s", wifi_config.sta.ssid, wifi_config.sta.password);
}