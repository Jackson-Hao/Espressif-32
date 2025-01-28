# HTTPS OTA patch for HUAWEI IoT platform
This is a patch for ESP-IDF's `esp_https_ota` component to support HUAWEI IoT platform.

What be patched:
- Fork `iot_esp_https_ota_begin` and `iot_esp_https_ota` from native `esp_https_ota` component, and rename them to `esp_https_ota_begin` and `esp_https_ota` respectively.
- Add HTTP Header `Authorization : Bearer <token>` to the request header.