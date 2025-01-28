#include "iot_ota.h"
#include "mqtt_config.h"
#include "iot_esp_https_ota.h"
#include "osThread.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "mqtt_client.h"
#include "version.h"
#define TAG "OTA"

static char ota_url[256];
static char ota_remote_version[32];
static char ota_token[128];

static ota_event_finish_callback ota_callback = NULL;
static bool isCurrentOTA = false;

UpdateStatus status = {
    .status = 0,
    .version = NULL,
};

char* OTA_get_current_version(void) {
    return APP_VERSION;
}

char* generate_ota_status(const char* object_device_id, int result_code, const char* version) {
    // 创建根对象
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        ESP_LOGE("JSON_CREATE", "Failed to create root object");
        return NULL;
    }

    // 添加 object_device_id
    cJSON_AddStringToObject(root, "object_device_id", object_device_id);

    // 创建 services 数组
    cJSON *services = cJSON_CreateArray();
    if (services == NULL) {
        ESP_LOGE("JSON_CREATE", "Failed to create services array");
        cJSON_Delete(root);
        return NULL;
    }
    cJSON_AddItemToObject(root, "services", services);

    // 创建服务对象
    cJSON *service = cJSON_CreateObject();
    if (service == NULL) {
        ESP_LOGE("JSON_CREATE", "Failed to create service object");
        cJSON_Delete(root);
        return NULL;
    }
    cJSON_AddItemToArray(services, service);

    // 添加 service_id 和 event_type
    cJSON_AddStringToObject(service, "service_id", "$ota");
    cJSON_AddStringToObject(service, "event_type", "upgrade_progress_report");

    // 创建 paras 对象
    cJSON *paras = cJSON_CreateObject();
    if (paras == NULL) {
        ESP_LOGE("JSON_CREATE", "Failed to create paras object");
        cJSON_Delete(root);
        return NULL;
    }
    cJSON_AddItemToObject(service, "paras", paras);

    // 添加 result_code 和 version
    cJSON_AddNumberToObject(paras, "result_code", result_code);
    cJSON_AddStringToObject(paras, "version", version);

    // 将 JSON 对象转换为字符串
    char *json_string = cJSON_PrintUnformatted(root);
    if (json_string == NULL) {
        ESP_LOGE("JSON_CREATE", "Failed to print JSON string");
        cJSON_Delete(root);
        return NULL;
    }

    cJSON_Delete(root);

    return json_string;
}

static void OTATaskEntry(void *arg) {
    esp_http_client_config_t http_config = {
        .url = ota_url,
        .timeout_ms = 10*1000,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .keep_alive_enable = true,
        .skip_cert_common_name_check = true,
    };

    char *Auth = malloc(strlen(ota_token) + 10);
    snprintf(Auth, strlen(ota_token) + 10, "Bearer %s", ota_token);
    ESP_LOGI(TAG, "OTA URL: %s", ota_url);
    ESP_LOGI(TAG, "Authorization: %s", Auth);

    esp_https_ota_head_t HEAD = {
        .Content_Type = "application/octet-stream",
        .Authorization = Auth,
    };

    esp_https_ota_config_t config = {
        .http_config = &http_config,
    };

    esp_err_t ota_finish_ret = iot_esp_https_ota(&config, &HEAD);
    if (ota_finish_ret == ESP_OK) {
        if(ota_callback) {
            status.status = 0;
            status.version = (ota_remote_version);
            ota_callback(&status);
            ESP_LOGI(TAG, "Restarting system...");
            vTaskDelay(pdMS_TO_TICKS(5000));
            esp_restart(); 
        } else {
            ESP_LOGE(TAG, "OTA Failed");
            status.status = 10;
            status.version = (ota_remote_version);
            ota_callback(&status);
        }
    } else {
        ESP_LOGE(TAG, "OTA Failed");
        status.status = 10;
        status.version = (ota_remote_version);
        ota_callback(&status);
    }
    isCurrentOTA = false;
    free(Auth);
    vTaskDelete(NULL);
}

uint8_t OTA_if_ota_topic(char* topic) {
    char topic_to_compare[64];
    snprintf(topic_to_compare, sizeof(topic_to_compare), IOT_OC_MQTT_PROFILE_OTA_TOPICFMT, DEVICE_ID);
    if (strstr(topic, topic_to_compare) != NULL) {
        return 1;
    }
    return 0;
}

esp_err_t OTA_info_config(const char* url, const char* remote_version, const char* token, ota_event_finish_callback callback) {
    snprintf(ota_url, sizeof(ota_url), "%s", url);
    snprintf(ota_remote_version, sizeof(ota_remote_version), "%s", remote_version);
    snprintf(ota_token, sizeof(ota_token), "%s", token);
    ota_callback = callback;
    return ESP_OK;
}

esp_err_t OTA_start() {
    if(isCurrentOTA) {
        return ESP_FAIL;
    }
    osThreadAttr_t attr;

    attr.name = "OTATask";
    attr.stack_size = 1024 * 8;
    attr.priority = 4;
    attr.cpu_id = 1;

    if((osThreadCreate(&attr, OTATaskEntry, NULL)) != 0) {
        ESP_LOGE(TAG, "create OTATask failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}