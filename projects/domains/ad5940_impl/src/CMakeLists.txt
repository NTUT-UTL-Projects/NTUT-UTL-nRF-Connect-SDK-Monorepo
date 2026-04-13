if(NOT TARGET ad5940_impl)
    add_library(ad5940_impl)
endif()

# Libraries
if(NOT TARGET ad5940_apps)
    add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../utils/ad5940_apps
        ad5940_apps
    )
endif()
target_link_libraries(
    ad5940_impl
    PUBLIC
        ad5940_apps
)

# Headers
target_precompile_headers(
    ad5940_impl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ad5940_impl.h
)

target_include_directories(
    ad5940_impl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Sources
add_subdirectory(intc)
add_subdirectory(intc_lock)
add_subdirectory(lib)
