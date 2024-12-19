#ifndef __MQTT_PROFILE_PACKAGE_H__
#define __MQTT_PROFILE_PACKAGE_H__


typedef struct{
    char voltage[10];
    char current[10];
} adc_t;

typedef struct {
    char *request_id;
    char *payload;
} cmd_t;

typedef enum {
    EN_OC_MQTT_PROFILE_VALUE_INT = 0,
    EN_OC_MQTT_PROFILE_VALUE_LONG,
    EN_OC_MQTT_PROFILE_VALUE_FLOAT,
    EN_OC_MQTT_PROFILE_VALUE_DOUBLE,
    EN_OC_MQTT_PROFILE_VALUE_STRING,           ///< must be ended with '\0'
    EN_OC_MQTT_PROFILE_VALUE_LAST,
}en_oc_profile_data_t;


typedef struct {
    void                 *nxt;   ///< ponit to the next key
    char                 *key;
    en_oc_profile_data_t  type;
    void                 *value;
}oc_mqtt_profile_kv_t;

typedef struct {
    void *nxt;
    char *service_id;                         ///< the service id in the profile, which could not be NULL
    char *event_time;                         ///< eventtime, which could be NULL means use the platform time
    oc_mqtt_profile_kv_t *service_property;   ///< the property in the profile, which could not be NULL
}oc_mqtt_profile_service_t;

typedef struct {
    int     ret_code;           ///< response code, 0 success while others failed
    char   *ret_name;           ///< response description,maybe used when failed
    char   *request_id;         ///< specified by the message command
    oc_mqtt_profile_kv_t  *paras;///< the command paras
}oc_mqtt_profile_cmdresp_t;

uint8_t oc_mqtt_profile_cmdresp(esp_mqtt_client_handle_t *mqtt_handle,char *oc_mqtt_profile_cmdresp_topic,char *deviceid, oc_mqtt_profile_cmdresp_t *payload);
int oc_mqtt_profile_propertyreport(esp_mqtt_client_handle_t *mqtt_handle, char *oc_mqtt_profile_propertyreport_topic, char *deviceid, oc_mqtt_profile_service_t *payload);
char *oc_mqtt_profile_package_cmdresp(oc_mqtt_profile_cmdresp_t *payload);
char *topic_make(char *fmt, char *device_id, char *request_id);

#endif
