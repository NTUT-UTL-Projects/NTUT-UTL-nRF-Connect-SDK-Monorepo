if(NOT TARGET ad5940_controller)
    add_library(ad5940_controller)
endif()

# ========================================
# Library

# ad5940_apps
if(NOT TARGET ad5940_apps)
    add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../utils/ad5940_apps/src
        ${CMAKE_CURRENT_BINARY_DIR}/aad5940_lib
    )
endif()
target_link_libraries(
    ad5940_controller
    PUBLIC
        ad5940_apps
)

# electrochemical_dev_kit
if(NOT TARGET electrochemical_dev_kit)
    FetchContent_Declare(
        electrochemical_dev_kit
        URL https://github.com/XIAN-SHENG-576692/Electrochemical-Dev-Kit/releases/download/latest/src.zip
    )
    FetchContent_MakeAvailable(electrochemical_dev_kit)
endif()

target_compile_definitions(
    electrochemical_dev_kit
        PUBLIC ELECTROCHEMICAL_DEV_KIT_BOOLEAN_TYPE=bool
        PUBLIC ELECTROCHEMICAL_DEV_KIT_ENERGY_TYPE=float
        PUBLIC ELECTROCHEMICAL_DEV_KIT_INDEX_TYPE=uint8_t
        PUBLIC ELECTROCHEMICAL_DEV_KIT_NUMBER_TYPE=uint8_t
        PUBLIC ELECTROCHEMICAL_DEV_KIT_RATE_TYPE=float
        PUBLIC ELECTROCHEMICAL_DEV_KIT_TIME_TYPE=float
)

target_link_libraries(
    ad5940_controller
    PUBLIC
        electrochemical_dev_kit
)

# ========================================
# Sources

# Header
target_precompile_headers(
    ad5940_controller
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ad5940_controller.h
)

target_include_directories(
    ad5940_controller
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Subdirectories
add_subdirectory(cal)
add_subdirectory(electrochemical)
add_subdirectory(event)
add_subdirectory(reset)
add_subdirectory(temperature)
add_subdirectory(trigger)
