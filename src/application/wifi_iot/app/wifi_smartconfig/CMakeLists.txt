FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_smartconfig/*.*)

idf_component_register(
    SRCS ${app_sources}

    INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_smartconfig/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_smartconfig/lib/iot_wifi_config/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/wifi_smartconfig/lib/osThread/include
)
