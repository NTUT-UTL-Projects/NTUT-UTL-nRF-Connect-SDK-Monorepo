add_library(ad5940_intc_impl)

file(GLOB C_FILES CONFIGURE_DEPENDS "*.c")
file(GLOB H_FILES CONFIGURE_DEPENDS "*.h")

target_sources(
    ad5940_intc_impl
    PRIVATE
        ${C_FILES}
)
target_precompile_headers(
    ad5940_intc_impl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ad5940_intc_impl.h
)
target_include_directories(
    ad5940_intc_impl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(
    ad5940_intc_impl
    PUBLIC
        ad5940_intc
)
