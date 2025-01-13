#include <stdio.h>
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "iot_cloud_oc.h"
#include "mqtt_client.h"
#include "mqtt_profile_package.h"
#include "iot_gpio_config.h"
#include "sys/unistd.h"
#include "iot_ota.h"

#define TAG "[MQTT client]:"
#define IOT_OC_MQTT_PROFILE_PROPERTYREPORT_TOPICFMT "$oc/devices/%s/sys/properties/report"
#define IOT_OC_MQTT_PROFILE_CMDRESP_TOPICFMT "$oc/devices/%s/sys/commands/response/request_id=%s"
#define IOT_OC_MQTT_PROFILE_CMDSUBSCRIBE_TOPICFMT "$oc/devices/%s/sys/commands/#"

static int get_request_id(char *topic, char* request_id, size_t request_id_size);
static void oc_cmdresp(cmd_t *cmd, int cmdret);
static void deal_cmd_msg(cmd_t *cmd);
static void deal_report_msg(void);
void IoTPropertyReportTask(void *pvParameters);
void IoTGpioMainTask(void *pvParameters);
static void IoTCloudMainTask(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void IoTCloudServiceTaskEntry(const char *mqtt_url, const char *client_id, const char *mqtt_user, const char *mqtt_pass);
static esp_mqtt_client_handle_t mqtt_client_handle = NULL;

adc_t adc_data;
gpio_ctl_t gpio_ctl;

int msg_id[3] = {0, 0, 0};  // msg_id[0]: publish, msg_id[1]: cmd, msg_id[2]: OTA

static int get_request_id(char *topic, char* request_id, size_t request_id_size) {
    const char* start = strstr(topic, "request_id=");
    if (start == NULL) {
        return -1;
    }
    start += strlen("request_id=");
    size_t length = strcspn(start, "/\0");
    if (length >= request_id_size) {
        return -1;
    }

    strncpy(request_id, start, length);
    request_id[length] = '\0';

    return 0;
}

static void oc_cmdresp(cmd_t *cmd, int cmdret) {
    uint8_t ret;
    oc_mqtt_profile_cmdresp_t cmdresp;
    ///< do the response
    cmdresp.paras = NULL;
    cmdresp.request_id = cmd->request_id;
    cmdresp.ret_code = cmdret;
    cmdresp.ret_name = NULL;
    if ((ret = oc_mqtt_profile_cmdresp(&mqtt_client_handle,IOT_OC_MQTT_PROFILE_CMDRESP_TOPICFMT,MQTT_CLIENT_ID, &cmdresp)) < 0) {
        printf("Fatal error: Failed to response command\n");
    }
    return;
}

#include "cJSON.h"
static void deal_cmd_msg(cmd_t *cmd) {
    cJSON *obj_root;
    cJSON *obj_cmdname;
    cJSON *obj_paras;
    cJSON *obj_para;
    int cmdret = 1;

    obj_root = cJSON_Parse(cmd->payload);
    if (obj_root == NULL) {
        oc_cmdresp(cmd, cmdret);
    }
    obj_cmdname = cJSON_GetObjectItem(obj_root, "command_name");
    if (obj_cmdname == NULL) {
        cJSON_Delete(obj_root);
    }
    if (strcmp(cJSON_GetStringValue(obj_cmdname), "LED_Control") == 0) {
        obj_paras = cJSON_GetObjectItem(obj_root, "paras");
        if (obj_paras == NULL) {
            cJSON_Delete(obj_root);
        }
        obj_para = cJSON_GetObjectItem(obj_paras, "status");
        if (obj_para == NULL) {
            cJSON_Delete(obj_root);
        }
        if (strcmp(cJSON_GetStringValue(obj_para), "ON") == 0) {
            gpio_ctl.gpio_pin = GPIO_LED_IO;
            gpio_ctl.gpio_status = 1;
            printf("Led On!\r\n");
        } else if (strcmp(cJSON_GetStringValue(obj_para), "OFF") == 0) {
            gpio_ctl.gpio_pin = GPIO_LED_IO;
            gpio_ctl.gpio_status = 0;
            printf("Led Off!\r\n");
        }
        else {
            gpio_ctl.gpio_pin = GPIO_LED_IO;
            gpio_ctl.gpio_status = 3;
            printf("Led Sparkling!\r\n");
        }
        cmdret = 0;
        oc_cmdresp(cmd, cmdret);
    } else if (strcmp(cJSON_GetStringValue(obj_cmdname), "Alarm") == 0) {
        obj_paras = cJSON_GetObjectItem(obj_root, "paras");
        if (obj_paras == NULL) {
            cJSON_Delete(obj_root);
        }
        obj_para = cJSON_GetObjectItem(obj_paras, "status");
        if (obj_para == NULL) {
            cJSON_Delete(obj_root);
        }
        if (strcmp(cJSON_GetStringValue(obj_para), "ON") == 0) {
            printf("Alarm On!\r\n");
        } else if (strcmp(cJSON_GetStringValue(obj_para), "OFF") == 0) {
            printf("Alarm Off!\r\n");
        }
        cmdret = 0;
        oc_cmdresp(cmd, cmdret);
    } else {
        return;
    }
}

static void deal_report_msg(void) {
    int ret;
    oc_mqtt_profile_service_t service;
    oc_mqtt_profile_kv_t voltage;
    oc_mqtt_profile_kv_t current;

    service.event_time = NULL;
    service.service_id = "ADC";
    service.service_property = &voltage;
    service.nxt = NULL;

    voltage.key = "voltage";
    voltage.value = adc_data.voltage;
    voltage.type = EN_OC_MQTT_PROFILE_VALUE_STRING;
    voltage.nxt = &current;

    current.key = "current";
    current.value = adc_data.current;
    current.type = EN_OC_MQTT_PROFILE_VALUE_STRING;
    current.nxt = NULL;
    
    ret = oc_mqtt_profile_propertyreport(&mqtt_client_handle, IOT_OC_MQTT_PROFILE_PROPERTYREPORT_TOPICFMT, MQTT_CLIENT_ID, &service);
    if (ret < 0) {
        printf("Fatal error: Failed to report property\n");
    }
    return;
}

void IoTGpioLedMainTask(void *pvParameters) {
    while(1) {
        if (gpio_ctl.gpio_status == 1) {
            gpio_set_level(gpio_ctl.gpio_pin, 1);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        } else if (gpio_ctl.gpio_status == 0) {
            gpio_set_level(gpio_ctl.gpio_pin, 0);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        } else {
            gpio_set_level(gpio_ctl.gpio_pin, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(gpio_ctl.gpio_pin, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
}

void IoTPropertyReportTask(void *pvParameters) {
    int voltage, current;
    while (1) {
        // Fake data
        voltage = rand() % 100;
        current = rand() % 100;
        snprintf(adc_data.voltage, sizeof(adc_data.voltage), "%d", voltage);
        snprintf(adc_data.current, sizeof(adc_data.current), "%d", current);
        deal_report_msg();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

//! ONLY FOR HUAWEI CLOUD IOT PLATFORM
void ota_event_finish_callback_default(UpdateStatus* status) {
    ESP_LOGI(TAG, "OTA status: %d, version: %s", status->status, status->version);
    if(status->status == 0) {
        char *json_str = generate_ota_status(DEVICE_ID, 0, status->version);
        if (json_str == NULL) {
            ESP_LOGE(TAG, "Failed to generate JSON string");
            return;
        }
        int ret = esp_mqtt_client_publish(mqtt_client_handle, IOT_OC_MQTT_PROFILE_OTA_TOPICFMT_UP, json_str, 0, 1, 0);
        if (ret < 0) {
            ESP_LOGE(TAG, "Failed to publish message");
        } else {
            ESP_LOGI(TAG, "OTA success");
        }
        free(json_str);
    } else {
        char *json_str = generate_ota_status(DEVICE_ID, status->status, status->version);
        if (json_str == NULL) {
            ESP_LOGE(TAG, "Failed to generate JSON string");
            return;
        }
        int ret = esp_mqtt_client_publish(mqtt_client_handle, IOT_OC_MQTT_PROFILE_OTA_TOPICFMT_UP, json_str, 0, 1, 0);
        if (ret < 0) {
            ESP_LOGE(TAG, "Failed to publish message");
        } else {
            ESP_LOGE(TAG, "OTA failed");
        }
        free(json_str);
    }
}

/*-------------------------------------------------------------------------------------*/
/*---------------------------------IoT Cloud Main Task---------------------------------*/
/*-------------------------------------------------------------------------------------*/

static void IoTCloudMainTask(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    esp_mqtt_event_handle_t data = (esp_mqtt_event_handle_t) event_data;
    char *topic_publish = NULL;
    char *topic_subscribe = NULL;
    char *topic_ota = NULL;

    switch ((esp_mqtt_event_id_t) event_id) {
        case MQTT_EVENT_CONNECTED:  // MQTT连接成功,订阅主题
            ESP_LOGI(TAG, "MQTT client connected");
            // 发布属性上报订阅主题
            topic_publish = topic_make(IOT_OC_MQTT_PROFILE_PROPERTYREPORT_TOPICFMT, MQTT_CLIENT_ID, NULL);
            ESP_LOGI(TAG, "topic_publish: %s", topic_publish);
            msg_id[0] = esp_mqtt_client_publish(mqtt_client_handle, topic_publish , NULL, 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id[0]);

            topic_subscribe = topic_make(IOT_OC_MQTT_PROFILE_CMDSUBSCRIBE_TOPICFMT, MQTT_CLIENT_ID, NULL);
            ESP_LOGI(TAG, "topic_subscribe: %s", topic_subscribe);
            msg_id[1] = esp_mqtt_client_subscribe_single(mqtt_client_handle, topic_subscribe, 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id[1]);

            topic_ota = topic_make(IOT_OC_MQTT_PROFILE_OTA_TOPICFMT, MQTT_CLIENT_ID, NULL);
            ESP_LOGI(TAG, "topic_ota: %s", topic_ota);
            msg_id[2] = esp_mqtt_client_subscribe_single(mqtt_client_handle, topic_ota, 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id[2]);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT client disconnected");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT client subscribed");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT client published");
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT client received data");
            // ESP_LOGI(TAG, "TOPIC=%.*s\r\n", data->topic_len, data->topic);
            char* payload = malloc(data->data_len + 1);
            char* topic = malloc(data->topic_len + 1);
            if (topic != NULL && payload != NULL) {
                strncpy(topic, data->topic, data->topic_len);
                topic[data->topic_len] = '\0';
                ESP_LOGI(TAG, "Topic=%s", topic);
                strncpy(payload, data->data, data->data_len);
                payload[data->data_len] = '\0';
                ESP_LOGI(TAG, "Data=%s", payload);
                //OTA Function
                if(OTA_if_ota_topic(topic) == 1) {  // OTA topic
                    printf("OTA topic\n"); 
                    char* event_type = extract_event_type(payload);
                    if (strcmp(event_type, "version_query") == 0) {
                        int ret =  oc_mqtt_send_version_report(&mqtt_client_handle, MQTT_BROKER_USER, OTA_get_current_version());
                        printf("Send version report\n");
                        if (ret < 0) {
                            printf("Failed to send version report\n");
                        }
                        free (payload);
                        free (topic);
                        break;
                    }
                    if (strcmp(event_type, "firmware_upgrade") == 0) {
                        printf("Firmware upgrade\n");
                        FirmwareInfo* firmware_info = extract_firmware_info(payload);
                        if (firmware_info == NULL) {
                            printf("Failed to extract firmware info\n");
                        } else {
                            printf("Extracted firmware info: version=%s, url=%s, token=%s\n", firmware_info->version, firmware_info->url, firmware_info->token);
                            esp_err_t ret = OTA_info_config(firmware_info->url, firmware_info->version, firmware_info->token, ota_event_finish_callback_default);
                            if (ret != ESP_OK) {
                                printf("Failed to config OTA address\n");
                            } else {
                                ret = OTA_start();
                                if (ret != ESP_OK) {
                                    printf("Failed to start OTA\n");
                                }
                            }
                        }
                    } else {
                    }
                } else {    // Command topic
                    char request_id[40];
                    if (get_request_id(topic, request_id, sizeof(request_id)) == 0) {
                        printf("Extracted request_id: %s\n", request_id);
                    } else {
                        printf("Failed to extract request_id\n");
                    }
                    cmd_t cmd = {request_id, payload};
                    deal_cmd_msg(&cmd);
                    free (payload);
                    free (topic);
                }
            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", (int)event_id);
            break;
    }
}

void IoTCloudServiceTaskEntry(const char *mqtt_url, const char *client_id, const char *mqtt_user, const char *mqtt_pass){
    esp_mqtt_client_config_t mqtt_cfg = {0};
    mqtt_cfg.broker.address.uri = mqtt_url;
    mqtt_cfg.broker.address.port = 1883;
    mqtt_cfg.credentials.client_id = client_id;
    mqtt_cfg.credentials.username = mqtt_user;
    mqtt_cfg.credentials.authentication.password = mqtt_pass;
    mqtt_cfg.task.priority = 12;
    mqtt_cfg.task.stack_size = 1024*8;
    mqtt_client_handle = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(mqtt_client_handle, ESP_EVENT_ANY_ID, IoTCloudMainTask, NULL);
    esp_mqtt_client_start(mqtt_client_handle);
}