# ========================================
# Libraries

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)

# ad5940_apps
add_subdirectory(
    ${ROOT_DIR}/utils/ad5940_apps/src
    ${CMAKE_CURRENT_BINARY_DIR}/ad5940_apps
)
target_compile_definitions(
    app
    PUBLIC
        CHIPSEL_594X
)

# ad5940_controller
add_subdirectory(
    ${ROOT_DIR}/domains/ad5940_controller/src
    ${CMAKE_CURRENT_BINARY_DIR}/ad5940_controller
)

# ad5940_impl
add_subdirectory(
    ${ROOT_DIR}/utils/ad5940_impl/src
    ${CMAKE_CURRENT_BINARY_DIR}/ad5940_impl
)