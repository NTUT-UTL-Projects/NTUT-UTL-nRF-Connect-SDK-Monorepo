# ========================================
# Libraries

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)

# ad5940_apps
add_subdirectory(
    ${ROOT_DIR}/utils/ad5940_apps/src
    ${CMAKE_CURRENT_BINARY_DIR}/ad5940_apps
)
target_link_libraries(
    demo
    PUBLIC
        ad5940_apps
)

# ad5940_controller
add_subdirectory(
    ${ROOT_DIR}/domains/ad5940_controller/src
    ${CMAKE_CURRENT_BINARY_DIR}/ad5940_controller
)
target_link_libraries(
    demo
    PUBLIC
        ad5940_controller
)

# ad5940_intc
add_subdirectory(
    ${ROOT_DIR}/utils/ad5940_intc/src
    ${CMAKE_CURRENT_BINARY_DIR}/ad5940_intc
)
target_link_libraries(
    demo
    PUBLIC
        ad5940_intc
)

# ad5940_lib
add_subdirectory(
    ${ROOT_DIR}/utils/ad5940_lib_impl/src
    ${CMAKE_CURRENT_BINARY_DIR}/ad5940_lib_impl
)
target_link_libraries(
    demo
    PUBLIC
        ad5940_lib
)
target_compile_definitions(
    ad5940_lib
    PUBLIC
        CHIPSEL_594X
)

# my_commands
target_link_libraries(
    demo
    PUBLIC
        my_commands
)
