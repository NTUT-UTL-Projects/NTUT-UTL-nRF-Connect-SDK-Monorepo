file(GLOB_RECURSE SRC_C_FILES CONFIGURE_DEPENDS "*.c")
file(GLOB_RECURSE SRC_H_FILES CONFIGURE_DEPENDS "*.h")

target_sources(
    app
    PRIVATE
        ${SRC_C_FILES}
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${SRC_H_FILES}
)

