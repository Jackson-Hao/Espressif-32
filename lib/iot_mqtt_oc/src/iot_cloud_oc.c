#include <stdio.h>
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "iot_cloud_oc.h"
#include "mqtt_client.h"
#include "mqtt_profile_package.h"
#include "iot_gpio_config.h"
#include "sys/unistd.h"

#define TAG "[MQTT client]:"
// #define CN_OC_MQTT_PROFILE_MESSAGE_TOPICFMT "$oc/devices/%s/sys/messages/down"
#define MQTT_CLIENT_ID "6613b0b82ccc1a58388044e4_Hi3861V100_0_0_2024111707"
#define CN_OC_MQTT_PROFILE_PROPERTYREPORT_TOPICFMT "$oc/devices/%s/sys/properties/report"
#define CN_OC_MQTT_PROFILE_CMDRESP_TOPICFMT "$oc/devices/%s/sys/commands/response/request_id=%s"

static int get_request_id(char *topic, char* request_id, size_t request_id_size);
static void oc_cmdresp(cmd_t *cmd, int cmdret);
static void deal_cmd_msg(cmd_t *cmd);
static void deal_report_msg(void);
void IoTPropertyReportTask(void *pvParameters);
void IoTGpioMainTask(void *pvParameters);
static void IoTCloudMainTask(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void IoTCloudServiceTaskEntry(const char *mqtt_url, const char *client_id, const char *mqtt_user, const char *mqtt_pass);

static esp_mqtt_client_handle_t     mqtt_client_handle = NULL;

adc_t adc_data;
gpio_ctl_t gpio_ctl;

uint8_t DHT11Data[4]={0};
uint8_t Temp, Humi;


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
    oc_mqtt_profile_cmdresp_t cmdresp;
    ///< do the response
    cmdresp.paras = NULL;
    cmdresp.request_id = cmd->request_id;
    cmdresp.ret_code = cmdret;
    cmdresp.ret_name = NULL;
    oc_mqtt_profile_cmdresp(&mqtt_client_handle,CN_OC_MQTT_PROFILE_CMDRESP_TOPICFMT,MQTT_CLIENT_ID, &cmdresp);
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
    } else {
    }
}

static void deal_report_msg(void) {
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
    
    oc_mqtt_profile_propertyreport(&mqtt_client_handle, CN_OC_MQTT_PROFILE_PROPERTYREPORT_TOPICFMT, MQTT_CLIENT_ID, &service);

    return;
}

static void DelayUs(uint32_t nCount) {
    usleep(nCount);
}  
static uint8_t DHT11_ReadValue(void) { 
    uint8_t i, sbuf = 0;
    for (i = 8; i > 0; i--) {
        sbuf <<= 1; 
        while (!gpio_get_level(DHT11_PIN));
        DelayUs(30);  // 延时 30us 后检测数据线是否还是高电平 
        if (gpio_get_level(DHT11_PIN)) {
            sbuf |= 1;  
        } else {
            sbuf |= 0;
        }
        while (gpio_get_level(DHT11_PIN));
    }
    return sbuf;   
}

static uint8_t DHT11_ReadTemHum(uint8_t *buf) {
    uint8_t check;

    buf[0] = DHT11_ReadValue();
    buf[1] = DHT11_ReadValue();
    buf[2] = DHT11_ReadValue();
    buf[3] = DHT11_ReadValue();
    
    check = DHT11_ReadValue();

    if (check == buf[0] + buf[1] + buf[2] + buf[3])
        return 1;
    else
        return 0;
}

void IoTGpioDHTMainTask(void* pvParameters) {
    printf("ESP32 DHT11 TEST:%s,%s!\r\n",__DATE__,__TIME__);
    esp_task_wdt_add(NULL);
    while(1) {
        gpio_set_direction(DHT11_PIN, GPIO_MODE_OUTPUT);
        gpio_set_level(DHT11_PIN, 0);
        DelayUs(19*1000);
        gpio_set_level(DHT11_PIN, 1);
        DelayUs(30);
        gpio_set_direction(DHT11_PIN, GPIO_MODE_INPUT);
        while(!gpio_get_level(DHT11_PIN));
        while(gpio_get_level(DHT11_PIN));

        if (DHT11_ReadTemHum(DHT11Data)) {
            Temp = DHT11Data[2];
            Humi = DHT11Data[0];
            printf("Humi:%d,Temp:%d\r\n",Humi,Temp);
        } else {
            printf("DHT11 Read Error!\r\n");
        }
        esp_task_wdt_reset();
        vTaskDelay(2500 / portTICK_PERIOD_MS);
    }
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
        voltage = rand() % 100;
        current = rand() % 100;
        snprintf(adc_data.voltage, sizeof(adc_data.voltage), "%d", voltage);
        snprintf(adc_data.current, sizeof(adc_data.current), "%d", current);
        deal_report_msg();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

static void IoTCloudMainTask(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    esp_mqtt_event_handle_t data = (esp_mqtt_event_handle_t) event_data;
    int msg_id = 0;
    //char *topic_subscribe = NULL;
    char *topic_publish = NULL;
    switch ((esp_mqtt_event_id_t) event_id) {
        case MQTT_EVENT_CONNECTED:  // MQTT连接成功,订阅主题
            ESP_LOGI(TAG, "MQTT client connected");
            topic_publish = topic_make(CN_OC_MQTT_PROFILE_PROPERTYREPORT_TOPICFMT, MQTT_CLIENT_ID, NULL);
            msg_id = esp_mqtt_client_publish(mqtt_client_handle, topic_publish , NULL, 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            // topic_subscribe = topic_make(CN_OC_MQTT_PROFILE_MESSAGE_TOPICFMT, MQTT_CLIENT_ID, NULL);
            // msg_id = esp_mqtt_client_subscribe(mqtt_client_handle, topic_subscribe, 1);
            // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
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
        case MQTT_EVENT_DATA:   // 接收到订阅的消息
            // ESP_LOGI(TAG, "topic->%s\n, data->%s\n", data->topic, data->data);
            char* payload = malloc(data->data_len + 1);
            char* topic = malloc(data->topic_len + 1);
            if (topic != NULL && payload != NULL) {
                strncpy(topic, data->topic, data->topic_len);
                topic[data->topic_len] = '\0';
                //ESP_LOGI(TAG, "Received topic: %s", topic);

                strncpy(payload, data->data, data->data_len);
                payload[data->data_len] = '\0';
                //ESP_LOGI(TAG, "Received data: %s", payload);
                
                char request_id[40];
                if (get_request_id(topic, request_id, sizeof(request_id)) == 0) {
                    //printf("Extracted request_id: %s\n", request_id);
                }
                else {
                    printf("Failed to extract request_id\n");
                }
                cmd_t cmd = {request_id, payload};
                deal_cmd_msg(&cmd);
                free (payload);
                free (topic);
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
    
    mqtt_client_handle = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(mqtt_client_handle, ESP_EVENT_ANY_ID, IoTCloudMainTask, NULL);
    esp_mqtt_client_start(mqtt_client_handle);
}