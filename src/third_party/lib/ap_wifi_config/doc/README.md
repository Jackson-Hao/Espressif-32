# AP_WIFI_CONFIG 接口文档

## 概述
`ap_wifi_config.c` 文件包含了用于配置和管理 WiFi 接入点 (AP) 和站点 (STA) 模式的函数。本文档将详细介绍这些函数的属性和使用方法。

## 函数列表

| 函数名 | 参数 | 返回值 | 描述 |
| --- | --- | --- | --- |
| `wifi_init_softap` | 无 | 无 | 初始化并启动 WiFi 接入点模式，设置 SSID、信道、最大连接数等参数，并启动 WiFi AP。 |
| `wifi_init_sta` | `char* ssid`: 目标 WiFi 网络的 SSID<br>`char* password`: 目标 WiFi 网络的密码 | 无 | 初始化并启动 WiFi 站点模式，设置目标网络的 SSID 和密码，并尝试连接到该网络。 |
| `get_mac_address` | 无 | `char*` - 设备的 MAC 地址字符串 | 获取设备的 MAC 地址，并返回一个包含 MAC 地址的字符串。 |
| `wifi_event_ap_handler` | `void* arg`: 事件参数<br>`esp_event_base_t event_base`: 事件基<br>`int32_t event_id`: 事件 ID<br>`void* event_data`: 事件数据 | 无 | 处理 WiFi 接入点的连接和断开事件，并记录相关日志。 |
| `wifi_event_sta_handler` | `void* arg`: 事件参数<br>`esp_event_base_t event_base`: 事件基<br>`int32_t event_id`: 事件 ID<br>`void* event_data`: 事件数据 | 无 | 处理 WiFi 站点的启动、断开和获取 IP 地址事件，并记录相关日志。 |

## 日志标签
- **TAG_AP**: `[WIFI_AP_TASK]:`
- **TAG_STA**: `[WIFI_STA_TASK]:`

## 依赖库
- `string.h`
- `freertos/FreeRTOS.h`
- `freertos/task.h`
- `esp_mac.h`
- `esp_wifi.h`
- `esp_event.h`
- `esp_log.h`
- `nvs_flash.h`
- `http_server_lite.h`
- `ping/ping_sock.h`
- `lwip/err.h`
- `lwip/sys.h`

## 注意事项
- 在调用任何 WiFi 初始化函数之前，请确保已正确初始化 NVS 和事件循环。
- 确保在使用 `get_mac_address` 函数后释放分配的内存。
