if(NOT TARGET ad5940_apps)
    add_library(ad5940_apps)
endif()

# Library
set(
    ROOT_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)
if(NOT TARGET ad5940_lib)
    add_subdirectory(
        ${ROOT_DIR}/utils/ad5940_lib/src
        ${CMAKE_CURRENT_BINARY_DIR}/aad5940_lib
    )
endif()
target_link_libraries(
    ad5940_apps
    PUBLIC
        ad5940_lib
)

# Sources
target_precompile_headers(
    ad5940_apps
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ad5940_apps.h
)
target_include_directories(
    ad5940_apps
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Subdirectories
add_subdirectory(commons)
add_subdirectory(consts)
add_subdirectory(str)
add_subdirectory(utils)
