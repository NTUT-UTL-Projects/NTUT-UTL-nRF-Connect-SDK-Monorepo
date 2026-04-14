file(GLOB SRC_C_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB SRC_H_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

target_sources(
    ad5940_impl
    PUBLIC
        ${SRC_C_FILES}
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${SRC_H_FILES}
)

if(CONFIG_MY_BUILD_ZEPHYR_BASE)
    add_subdirectory(zephyr)
elseif(CONFIG_MY_BUILD_STM32)
elseif(CONFIG_MY_BUILD_FAKE_IMPL)
    add_subdirectory(fake)
else()
    message(FATAL_ERROR "Unsupported MCU configuration")
endif()
