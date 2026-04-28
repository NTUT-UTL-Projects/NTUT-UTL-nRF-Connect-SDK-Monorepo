file(GLOB SRC_C_FILES CONFIGURE_DEPENDS "*.c")

target_sources(
    app
    PRIVATE
        ${SRC_C_FILES}
)

target_include_directories(
    app
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
