#ifndef __AP_WIFI_CONFIG_H__
#define __AP_WIFI_CONFIG_H__

void wifi_init_softap(void);
void wifi_init_sta(char* ssid, char* password);

#define AP_WIFI_SSID "DIRECT_ESP32_"
#define AP_WIFI_CHANNEL 4
#define AP_WIFI_MAX_STA_CONN 4

#endif // __AP_WIFI_CONFIG_H__