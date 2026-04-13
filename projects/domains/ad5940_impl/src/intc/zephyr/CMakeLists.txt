file(GLOB SRC_C_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

if(ZEPHYR_BASE)
    zephyr_library_include_directories(
        .
    )
    zephyr_library_sources(
        ${SRC_C_FILES}
    )
    set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../..)
    add_subdirectory(${ROOT_DIR}/utils/gpio/src gpio)
    target_link_libraries(app PRIVATE gpio)
endif()
