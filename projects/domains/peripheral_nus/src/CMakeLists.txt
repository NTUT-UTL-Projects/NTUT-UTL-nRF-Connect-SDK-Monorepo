file(GLOB SRC_H_FILES CONFIGURE_DEPENDS "*.h")

target_precompile_headers(
    app
    PUBLIC
        ${SRC_H_FILES}
)

target_include_directories(
    app
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_subdirectory(init)
add_subdirectory(received_data)
