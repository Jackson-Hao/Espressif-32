FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/uart_loopback/*.*)

idf_component_register(
    SRCS ${app_sources}

    INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/uart_loopback/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/uart_loopback/lib/osThread/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/uart_loopback/lib/iot_uart_ctl/include
)
