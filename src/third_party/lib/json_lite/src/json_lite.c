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
        ESP_LOGE("JSON_PARSE", "JSON Parse Error");
        return NULL;
    }

    jsonResolut_t *json_res = (jsonResolut_t *)malloc(sizeof(jsonResolut_t));
    if (json_res == NULL) {
        ESP_LOGE("JSON_PARSE", "Memory Error");
        cJSON_Delete(root);
        return NULL;
    }

    cJSON *ssid = cJSON_GetObjectItem(root, "ssid");
    if (ssid == NULL) {
        ESP_LOGE("JSON_PARSE", "ssid Error");
        free(json_res);
        cJSON_Delete(root);
        return NULL;
    }
    json_res->ssid = strdup(ssid->valuestring);

    cJSON *password = cJSON_GetObjectItem(root, "password");
    if (password == NULL) {
        ESP_LOGE("JSON_PARSE", "password Error");
        free(json_res->ssid);
        free(json_res);
        cJSON_Delete(root);
        return NULL;
    }
    json_res->password = strdup(password->valuestring);

    cJSON *client_ip = cJSON_GetObjectItem(root, "client_ip");
    if (client_ip == NULL) {
        ESP_LOGE("JSON_PARSE", "client_ip Error");
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

