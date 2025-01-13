#include "json_lite.h"

/*
{
    "ssid": "xxxx",
    "password": "xxxxx",
    "client_ip": "xxx.xxx.xxx.xxx",
}
*/

jsonResolut_t* json_parse_analyzed(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        ESP_LOGI("JSON_PARSE", "JSON Parse Error");
        return NULL;
    }

    jsonResolut_t *json_res = (jsonResolut_t *)malloc(sizeof(jsonResolut_t));
    if (json_res == NULL) {
        ESP_LOGI("JSON_PARSE", "Memory Error");
        cJSON_Delete(root);
        return NULL;
    }

    cJSON *ssid = cJSON_GetObjectItem(root, "ssid");
    if (ssid == NULL) {
        ESP_LOGI("JSON_PARSE", "ssid Error");
        free(json_res);
        cJSON_Delete(root);
        return NULL;
    }
    json_res->ssid = strdup(ssid->valuestring);

    cJSON *password = cJSON_GetObjectItem(root, "password");
    if (password == NULL) {
        ESP_LOGI("JSON_PARSE", "password Error");
        free(json_res->ssid);
        free(json_res);
        cJSON_Delete(root);
        return NULL;
    }
    json_res->password = strdup(password->valuestring);

    cJSON *client_ip = cJSON_GetObjectItem(root, "client_ip");
    if (client_ip == NULL) {
        ESP_LOGI("JSON_PARSE", "client_ip Error");
        free(json_res->ssid);
        free(json_res->password);
        free(json_res);
        cJSON_Delete(root);
        return NULL;
    }
    json_res->client_ip = strdup(client_ip->valuestring);

    cJSON_Delete(root);
    return json_res;
}

char* json_msg_ret(char* msg) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "message", msg);
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str;
}

char* extract_event_type(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        ESP_LOGI("JSON_PARSE", "JSON Parse Error");
        return NULL;
    }

    cJSON *services = cJSON_GetObjectItem(root, "services");
    if (services == NULL || !cJSON_IsArray(services)) {
        ESP_LOGI("JSON_PARSE", "services Error");
        cJSON_Delete(root);
        return NULL;
    }

    cJSON *service = cJSON_GetArrayItem(services, 0);
    if (service == NULL) {
        ESP_LOGI("JSON_PARSE", "service Error");
        cJSON_Delete(root);
        return NULL;
    }

    cJSON *event_type = cJSON_GetObjectItem(service, "event_type");
    if (event_type == NULL) {
        ESP_LOGI("JSON_PARSE", "event_type Error");
        cJSON_Delete(root);
        return NULL;
    }

    char *event_type_str = strdup(event_type->valuestring);
    cJSON_Delete(root);
    return event_type_str;
}

FirmwareInfo* extract_firmware_info(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        ESP_LOGI("JSON_PARSE", "JSON Parse Error");
        return NULL;
    }

    cJSON *services = cJSON_GetObjectItem(root, "services");
    if (services == NULL || !cJSON_IsArray(services)) {
        ESP_LOGI("JSON_PARSE", "services Error");
        cJSON_Delete(root);
        return NULL;
    }

    cJSON *service = cJSON_GetArrayItem(services, 0);
    if (service == NULL) {
        ESP_LOGI("JSON_PARSE", "service Error");
        cJSON_Delete(root);
        return NULL;
    }

    cJSON *paras = cJSON_GetObjectItem(service, "paras");
    if (paras == NULL) {
        ESP_LOGI("JSON_PARSE", "paras Error");
        cJSON_Delete(root);
        return NULL;
    }

    cJSON *version = cJSON_GetObjectItem(paras, "version");
    cJSON *url = cJSON_GetObjectItem(paras, "url");
    cJSON *sign = cJSON_GetObjectItem(paras, "access_token");
    if (version == NULL || url == NULL || sign == NULL) {
        ESP_LOGI("JSON_PARSE", "version, url, or sign Error");
        cJSON_Delete(root);
        return NULL;
    }

    FirmwareInfo *info = (FirmwareInfo *)malloc(sizeof(FirmwareInfo));
    if (info == NULL) {
        ESP_LOGI("JSON_PARSE", "Memory Allocation Error");
        cJSON_Delete(root);
        return NULL;
    }

    info->version = strdup(version->valuestring);
    info->url = strdup(url->valuestring);
    info->token = strdup(sign->valuestring);

    cJSON_Delete(root);
    return info;
}