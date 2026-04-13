file(GLOB SRC_H_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# Headers
target_precompile_headers(
    ad5940_impl
    PUBLIC
        ${SRC_H_FILES}
)

target_include_directories(
    ad5940_impl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

if(ZEPHYR_BASE)
    add_subdirectory(zephyr)
elseif(CONFIG_STM32)
elseif(BUILD_FAKE)
    add_subdirectory(fake)
else()
    message(FATAL_ERROR "Unsupported MCU configuration")
endif()
