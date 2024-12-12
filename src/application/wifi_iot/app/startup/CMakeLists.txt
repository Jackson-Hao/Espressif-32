FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/startup/*.*)

idf_component_register(
    SRCS ${app_sources}

    INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/startup/include
)
