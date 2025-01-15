FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/oled_ssd1306/*.*)

idf_component_register(
    SRCS ${app_sources}

    INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/oled_ssd1306/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/oled_ssd1306/lib/osThread/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/oled_ssd1306/lib/oled/include
)
