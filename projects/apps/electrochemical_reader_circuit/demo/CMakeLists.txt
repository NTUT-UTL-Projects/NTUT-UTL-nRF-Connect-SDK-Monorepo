add_executable(demo)

target_compile_options(
    demo
    PRIVATE
        -Wall
        -Werror
        -O3
)

# Sources
file(GLOB SRC_C_FILES CONFIGURE_DEPENDS "*.c")
file(GLOB SRC_H_FILES CONFIGURE_DEPENDS "*.h")

target_sources(
    demo
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

# Libraries
target_link_libraries(
    demo
    PRIVATE
        app
)
