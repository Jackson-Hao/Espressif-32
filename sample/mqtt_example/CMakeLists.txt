# src/wifi_iot/app/mqtt_example/CMakeLists.txt
FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/mqtt_example/*.*)

idf_component_register(
    SRCS ${app_sources}
    
    INCLUDE_DIRS 
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/mqtt_example/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/mqtt_example/lib/iot_mqtt_oc/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/mqtt_example/lib/iot_gpio_ctl/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/mqtt_example/lib/iot_wifi_connect/include
)