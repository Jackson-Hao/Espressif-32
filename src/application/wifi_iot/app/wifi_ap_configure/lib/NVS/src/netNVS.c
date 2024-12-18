#include "nvs_flash.h"
#include <string.h>
#include "netNVS.h"

void NVS_init() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
}

void NVS_save_wifi_config(char *ssid, char *password) {
    nvs_handle_t wifi_nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &wifi_nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        err = nvs_set_str(wifi_nvs_handle, "ssid", ssid);
        err = nvs_set_str(wifi_nvs_handle, "password", password);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        nvs_close(wifi_nvs_handle);
    }
}

nvs_wifi_config_t NVS_read_wifi_config(void){
    nvs_wifi_config_t wifi_config;
    nvs_handle_t wifi_nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &wifi_nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        size_t ssid_len = 0;
        size_t password_len = 0;
        err = nvs_get_str(wifi_nvs_handle, "ssid", NULL, &ssid_len);
        err = nvs_get_str(wifi_nvs_handle, "password", NULL, &password_len);
        if (err != ESP_OK) {
            printf("Failed to read!\n");
        } else {
            wifi_config.ssid = malloc(ssid_len);
            wifi_config.password = malloc(password_len);
            err = nvs_get_str(wifi_nvs_handle, "ssid", wifi_config.ssid, &ssid_len);
            err = nvs_get_str(wifi_nvs_handle, "password", wifi_config.password, &password_len);
            printf("Done\n");
        }
        nvs_close(wifi_nvs_handle);
    }
    return wifi_config;
}

esp_err_t ifWifiConfigExists(void) {
    nvs_handle_t wifi_nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &wifi_nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        size_t ssid_len = 0;
        size_t password_len = 0;
        err = nvs_get_str(wifi_nvs_handle, "ssid", NULL, &ssid_len);
        err = nvs_get_str(wifi_nvs_handle, "password", NULL, &password_len);
        if (err != ESP_OK) {
            printf("Failed to read!\n");
        } else {
            printf("Done\n");
            return ESP_OK;
        }
        nvs_close(wifi_nvs_handle);
    }
    return ESP_FAIL;
}