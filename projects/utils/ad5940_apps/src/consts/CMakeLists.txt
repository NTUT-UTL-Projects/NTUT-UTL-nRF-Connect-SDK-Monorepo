file(GLOB C_FILES CONFIGURE_DEPENDS "*.c")
file(GLOB H_FILES CONFIGURE_DEPENDS "*.h")

target_sources(
    ad5940_apps
    PRIVATE
        ${C_FILES}
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${H_FILES}
)

target_precompile_headers(
    ad5940_apps
    PUBLIC
        ${H_FILES}
)
