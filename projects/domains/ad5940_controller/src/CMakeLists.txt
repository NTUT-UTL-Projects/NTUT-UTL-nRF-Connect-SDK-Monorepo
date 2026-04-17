# ========================================
# Libraries

# ad5940_apps
# if(NOT TARGET ad5940_apps)
#     add_subdirectory(
#         ${CMAKE_CURRENT_SOURCE_DIR}/../../../utils/ad5940_apps
#         ad5940_apps
#     )
# endif()
# target_link_libraries(
#     app
#     PUBLIC
#         ad5940_apps
# )

# electrochemical_dev_kit
FetchContent_Declare(
    electrochemical_dev_kit
    URL https://github.com/XIAN-SHENG-576692/Electrochemical-Dev-Kit/releases/download/latest/src.zip
)
FetchContent_MakeAvailable(electrochemical_dev_kit)

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
    app
    PUBLIC
        electrochemical_dev_kit
)

# ========================================
# Headers
target_precompile_headers(
    app
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ad5940_controller.h
)

target_include_directories(
    app
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Sources
add_subdirectory(cal)
add_subdirectory(electrochemical)
add_subdirectory(event)
add_subdirectory(reset)
add_subdirectory(temperature)
add_subdirectory(trigger)
