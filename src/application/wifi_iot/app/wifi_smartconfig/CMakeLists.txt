FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/net_config/*.*)

idf_component_register(
    SRCS ${app_sources}

    INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/net_config/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/net_config/lib/iot_wifi_config/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/net_config/lib/osThread/include
)
