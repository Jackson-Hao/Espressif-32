#include "iot_wifi_config.h"

static const char *TAG = "[WIFI_SC_TASK]:";

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
    } else if(event_base == IP_EVENT) {
        switch(event_id) {
            case IP_EVENT_STA_GOT_IP:
                ESP_LOGI(TAG,"get ip address");
                break;
        }
    } else if (event_base == SC_EVENT) {
        switch (event_id) {
            case SC_EVENT_SCAN_DONE: {
                ESP_LOGI(TAG, "Scan done");
                break;
                }
            case SC_EVENT_GOT_SSID_PSWD: {
                smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
                wifi_config_t wifi_config = {0};
                
                memset(&wifi_config, 0, sizeof(wifi_config_t));

                sniprintf((char*)wifi_config.sta.ssid, sizeof(wifi_config.sta.ssid), "%s", (char*)evt->ssid);
                sniprintf((char*)wifi_config.sta.password, sizeof(wifi_config.sta.password), "%s", (char*)evt->password);
                ESP_LOGI(TAG, "SSID:%s", wifi_config.sta.ssid);

                wifi_config.sta.bssid_set = evt->bssid_set;
                if(wifi_config.sta.bssid_set) {
                    memcpy(wifi_config.sta.bssid, evt->bssid, 6);
                }
                esp_wifi_disconnect();
                esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
                esp_wifi_connect();
                ESP_LOGI(TAG, "smartconfig over");
                break;
                }
            case SC_EVENT_SEND_ACK_DONE:{
                esp_smartconfig_stop();
                break;
            }
            default: break;
        }
    }
}

esp_err_t wifi_config_init(void) {
    int ret;
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&wifi_event_handler,NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&wifi_event_handler,NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT,ESP_EVENT_ANY_ID,&wifi_event_handler,NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    esp_smartconfig_set_type(SC_TYPE_ESPTOUCH);
    smartconfig_start_config_t sc_cfg_start = SMARTCONFIG_START_CONFIG_DEFAULT();

    ret = esp_smartconfig_start(&sc_cfg_start);
    if(ret == ESP_FAIL) {
        ESP_LOGE(TAG, "smartconfig start failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}