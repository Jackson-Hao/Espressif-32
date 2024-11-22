#include "cJSON.h"
#include "mqtt_client.h"
#include "mqtt_profile_package.h"

#define CN_OC_MQTT_PROFILE_CMDRESP_KEY_RETCODE "result_code"
#define CN_OC_MQTT_PROFILE_CMDRESP_KEY_RESPNAME "response_name"
#define CN_OC_MQTT_PROFILE_CMDRESP_KEY_PARAS "paras"
#define CN_OC_MQTT_PROFILE_SETPROPERTYRESP_KEY_RETDESC "result_desc"
#define CN_OC_MQTT_PROFILE_SERVICES_KEY "services"
#define CN_OC_MQTT_PROFILE_SERVICE_KEY_SERVICEID "service_id"
#define CN_OC_MQTT_PROFILE_SERVICE_KEY_PROPERTIES "properties"
#define CN_OC_MQTT_PROFILE_SERVICE_KEY_EVENTTIME "event_time"

static void cJSON_Delete_root(cJSON *root);
static cJSON *profile_fmtvalue(oc_mqtt_profile_kv_t *kv);
static cJSON *make_kvs(oc_mqtt_profile_kv_t *kvlst);
static cJSON *make_service(oc_mqtt_profile_service_t *service_info);
static cJSON *make_services(oc_mqtt_profile_service_t *service_info);
char *topic_make(char *fmt, char *device_id, char *request_id);
int oc_mqtt_profile_cmdresp(esp_mqtt_client_handle_t *mqtt_handle, char *oc_mqtt_profile_cmdresp_topic, char *deviceid, oc_mqtt_profile_cmdresp_t *payload);
char *oc_mqtt_profile_package_propertyreport(oc_mqtt_profile_service_t *payload);
int oc_mqtt_profile_propertyreport(esp_mqtt_client_handle_t *mqtt_handle, char *oc_mqtt_profile_propertyreport_topic, char *deviceid, oc_mqtt_profile_service_t *payload);
char *oc_mqtt_profile_package_cmdresp(oc_mqtt_profile_cmdresp_t *payload);

static void cJSON_Delete_root(cJSON *root) {
    if (root != NULL) {
        cJSON_Delete(root);
        root = NULL;
    }
}

static cJSON *profile_fmtvalue(oc_mqtt_profile_kv_t *kv) {
    cJSON *ret = NULL;
    switch (kv->type) {
        case EN_OC_MQTT_PROFILE_VALUE_INT:
            ret = cJSON_CreateNumber((double)(*(int *)kv->value));
            break;
        case EN_OC_MQTT_PROFILE_VALUE_LONG:
            ret = cJSON_CreateNumber((double)(*(long *)kv->value));
            break;
        case EN_OC_MQTT_PROFILE_VALUE_FLOAT:
            ret = cJSON_CreateNumber((*(double *)kv->value));
            break;
        case EN_OC_MQTT_PROFILE_VALUE_STRING:
            ret = cJSON_CreateString((const char *)kv->value);
            break;
        default:
            break;
    }

    return ret;
}

static cJSON *make_kvs(oc_mqtt_profile_kv_t *kvlst) {
    cJSON *root;
    cJSON *kv;
    oc_mqtt_profile_kv_t *kv_info;

    ///< build a root node
    root = cJSON_CreateObject();
    if (root == NULL) {
        cJSON_Delete_root(root);
        return root;
    }

    ///< add all the property to the properties
    kv_info = kvlst;
    while (kv_info != NULL) {
        kv = profile_fmtvalue(kv_info);
        if (kv == NULL) {
            cJSON_Delete_root(root);
            return root;
        }

        cJSON_AddItemToObject(root, kv_info->key, kv);
        kv_info = kv_info->nxt;
    }

    ///< OK, now we return it
    return root;
}

static cJSON *make_service(oc_mqtt_profile_service_t *service_info) {
    cJSON *root;
    cJSON *service_id;
    cJSON *properties;
    cJSON *event_time;

    ///< build a root node
    root = cJSON_CreateObject();
    if (root == NULL) {
        cJSON_Delete_root(root);
        return root;
    }

    ///< add the service_id node to the root node
    service_id = cJSON_CreateString(service_info->service_id);
    if (service_id == NULL) {
        cJSON_Delete_root(root);
        return root;
    }
    cJSON_AddItemToObjectCS(root, CN_OC_MQTT_PROFILE_SERVICE_KEY_SERVICEID, service_id);

    ///< add the properties node to the root
    properties = make_kvs(service_info->service_property);
    if (properties == NULL) {
        cJSON_Delete_root(root);
        return root;
    }
    cJSON_AddItemToObjectCS(root, CN_OC_MQTT_PROFILE_SERVICE_KEY_PROPERTIES, properties);

    ///< add the event time (optional) to the root
    if (service_info->event_time != NULL) {
        event_time = cJSON_CreateString(service_info->event_time);
        if (event_time == NULL) {
            cJSON_Delete_root(root);
            return root;
        }
        cJSON_AddItemToObjectCS(root, CN_OC_MQTT_PROFILE_SERVICE_KEY_EVENTTIME, event_time);
    }

    ///< OK, now we return it
    return root;
}

static cJSON *make_services(oc_mqtt_profile_service_t *service_info) {
    cJSON *services = NULL;
    cJSON *service;
    oc_mqtt_profile_service_t *service_tmp;

    ///< create the services array node
    services = cJSON_CreateArray();
    if (services == NULL) {
        if (services != NULL) {
            cJSON_Delete(services);
            services = NULL;
        }
        return services;
    }

    service_tmp = service_info;
    while (service_tmp != NULL) {
        service = make_service(service_tmp);
        if (service == NULL) {
            if (services != NULL) {
                cJSON_Delete(services);
                services = NULL;
            }
            return services;
        }

        cJSON_AddItemToArray(services, service);
        service_tmp = service_tmp->nxt;
    }

    ///< now we return the services
    return services;
}

char *topic_make(char *fmt, char *device_id, char *request_id) {
    int len;
    char *ret = NULL;

    if (device_id == NULL) {
        return ret;
    }
    len = strlen(fmt) + strlen(device_id);
    if (request_id != NULL) {
        len += strlen(request_id);
    }

    ret = malloc(len);
    if (ret != NULL) {
        (void)snprintf(ret, len, fmt, device_id, request_id);
    }
    return ret;
}

int oc_mqtt_profile_cmdresp(esp_mqtt_client_handle_t *mqtt_handle,char *oc_mqtt_profile_cmdresp_topic,char *deviceid, oc_mqtt_profile_cmdresp_t *payload) {
    int ret = 1;
    char *topic;
    char *msg;
    char *device_id = deviceid;

    if ((payload == NULL) || (payload->request_id == NULL)) {
        return ret;
    }

    topic = topic_make(oc_mqtt_profile_cmdresp_topic, device_id, payload->request_id);
    //printf("topic set!\n");
    msg = oc_mqtt_profile_package_cmdresp(payload);
    //printf("msg set!\n");
    if ((topic != NULL) && (msg != NULL)) {
        ret = esp_mqtt_client_publish(*mqtt_handle, topic, msg, strlen(msg), 1, 0);
    } else {
        printf("Failed to publish message\n");
    }

    free(topic);
    free(msg);

    return ret;
}

char *oc_mqtt_profile_package_propertyreport(oc_mqtt_profile_service_t *payload) {
    char *ret = NULL;
    cJSON *root;
    cJSON *services;

    ///< create the root node
    root = cJSON_CreateObject();
    if (root == NULL) {
        cJSON_Delete_root(root);
        return ret;
    }

    ///< create the services array node to the root
    services = make_services(payload);
    if (services == NULL) {
        cJSON_Delete_root(root);
        return ret;
    }
    cJSON_AddItemToObjectCS(root, CN_OC_MQTT_PROFILE_SERVICES_KEY, services);

    ///< OK, now we make it to a buffer
    ret = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return ret;
}

int oc_mqtt_profile_propertyreport(esp_mqtt_client_handle_t *mqtt_handle, char *oc_mqtt_profile_propertyreport_topic, char *deviceid, oc_mqtt_profile_service_t *payload) {
    int ret = 1;
    char *topic;
    char *msg;
    char *device_id = deviceid;
    

    if ((payload == NULL) || (payload->service_id == NULL) || (payload->service_property == NULL)) {
        return ret;
    }

    topic = topic_make(oc_mqtt_profile_propertyreport_topic, device_id, NULL);
    msg = oc_mqtt_profile_package_propertyreport(payload);
    if ((topic != NULL) && (msg != NULL)) {
        ret = esp_mqtt_client_publish(*mqtt_handle, topic, msg, strlen(msg), 1, 0);
    } else {
        printf("Failed to publish message\n");
    }

    free(topic);
    free(msg);

    return ret;
}

char *oc_mqtt_profile_package_cmdresp(oc_mqtt_profile_cmdresp_t *payload) {
    char *ret = NULL;
    cJSON *root;
    cJSON *ret_code;
    cJSON *ret_name;
    cJSON *paras;

    root = cJSON_CreateObject();
    if (root == NULL) {
        cJSON_Delete_root(root);
        return ret;
    }

    ret_code = cJSON_CreateNumber(payload->ret_code);
    if (ret_code == NULL) {
        cJSON_Delete_root(root);
        return ret;
    }
    cJSON_AddItemToObjectCS(root, CN_OC_MQTT_PROFILE_CMDRESP_KEY_RETCODE, ret_code);

    if (payload->ret_name != NULL) {
        ret_name = cJSON_CreateString(payload->ret_name);
        if (ret_name == NULL) {
            cJSON_Delete_root(root);
            return ret;
        }
        cJSON_AddItemToObjectCS(root, CN_OC_MQTT_PROFILE_SETPROPERTYRESP_KEY_RETDESC, ret_name);
    }

    if (payload->paras != NULL) {
        paras = make_kvs(payload->paras);
        if (paras == NULL) {
            cJSON_Delete_root(root);
            return ret;
        }
        cJSON_AddItemToObjectCS(root, CN_OC_MQTT_PROFILE_CMDRESP_KEY_PARAS, paras);
    }

    ret = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return ret;
}

