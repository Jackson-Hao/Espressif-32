#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include <sys/param.h>
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "protocol_examples_utils.h"
#include "esp_tls_crypto.h"
#include "esp_http_server.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_check.h"
#include "ap_wifi_config.h"
#include "netNVS.h"

#include "json_lite.h"

#define EXAMPLE_HTTP_QUERY_KEY_MAX_LEN  (64)
static const char* TAG = "HTTP_SERVER_LITE";

static esp_err_t get_handler(httpd_req_t *req) {
    char*  buf;
    size_t buf_len;

    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG, "buffer alloc failed");
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        ESP_RETURN_ON_FALSE(buf, ESP_ERR_NO_MEM, TAG, "buffer alloc failed");
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
        }
        free(buf);
    }

    const char* resp_str = (const char*) req->user_ctx;
    ESP_LOGI(TAG, "Sending response: %s", resp_str);
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}

static const httpd_uri_t get_request = {
    .uri       = "/test",
    .method    = HTTP_GET,
    .handler   = get_handler,
    .user_ctx  = "Hello World!"
};

static esp_err_t post_handler(httpd_req_t *req) {
    char buf[100];
    int ret, remaining = req->content_len;

    while (remaining > 0) {
        if ((ret = httpd_req_recv(req, buf,
                        MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                continue;
            }
            return ESP_FAIL;
        }
        jsonResolut_t* json_ret;
        if((json_ret = json_parse_analyzed(buf)) == NULL){
            ESP_LOGE(TAG, "JSON Parse Error");
            const char* resp_str = json_msg_ret("Data Error");
            httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
            httpd_resp_send_chunk(req, NULL, 0);
            return ESP_FAIL;
        }
        const char* resp_str = json_msg_ret("Recieved");
        httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
        remaining -= ret;

        /* Log data received */
        ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
        ESP_LOGI(TAG, "%.*s", ret, buf);
        ESP_LOGI(TAG, "====================================\n\n");
        ESP_LOGI(TAG, "=========== RECEIVED CONFIG ==========");
        ESP_LOGI(TAG, "ssid: %s", json_ret->ssid);
        ESP_LOGI(TAG, "password: %s", json_ret->password);
        ESP_LOGI(TAG, "client_ip: %s", json_ret->client_ip);
        ESP_LOGI(TAG, "======================================");
        NVS_save_wifi_config(json_ret->ssid, json_ret->password);
        ESP_LOGI(TAG, "wifi configure success");
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    ESP_LOGI(TAG, "Device will restart in 5 seconds...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    esp_restart();
    return ESP_OK;
}

static const httpd_uri_t recieve_data = {
    .uri       = "/recieve",
    .method    = HTTP_POST,
    .handler   = post_handler,
    .user_ctx  = NULL
};

static esp_err_t shake_handler(httpd_req_t *req) {
    const char* resp_str = "{\"status\": \"success\"}";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static const httpd_uri_t shakehand = {
    .uri       = "/shakehand",
    .method    = HTTP_POST,
    .handler   = shake_handler,
    .user_ctx  = NULL
};

static httpd_handle_t start_webserver() {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &get_request);
        httpd_register_uri_handler(server, &recieve_data);
        httpd_register_uri_handler(server, &shakehand);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}

void httpd_server_start(void){
    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &server, NULL));

    server = start_webserver();
}