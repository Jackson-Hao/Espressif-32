FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/*.c)

idf_component_register(
    SRCS ${app_sources}

    INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/ap_wifi_config/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/osThread/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/http_server_lite/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/protocol_common/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/json_lite/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/NVS/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/iot_mqtt_oc/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/iot_gpio/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/OTA/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/lib/version/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure_with_ota/components/esp_https_ota_patched/include
    REQUIRES mbedtls
)
