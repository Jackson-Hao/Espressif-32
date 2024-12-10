FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/multi_task/*.*)

idf_component_register(
    SRCS ${app_sources}

    INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/multi_task/include
        ${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/multi_task/lib/osThread/include
)
