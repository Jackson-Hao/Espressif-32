# src/wifi_iot/app/blink/CMakeLists.txt
FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/blink/*.*)

idf_component_register(
    SRCS ${app_sources}

    INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/blink/include
)