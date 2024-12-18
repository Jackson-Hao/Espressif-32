FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure/*.*)

idf_component_register(
    SRCS ${app_sources}

    INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure/lib/ap_wifi_config/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure/lib/osThread/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure/lib/http_server_lite/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure/lib/protocol_common/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure/lib/json_lite/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_ap_configure/lib/NVS/include
)
