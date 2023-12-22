cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

if (NOT PICO_SDK_PATH)
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
endif ()

if (NOT PIMORONI_PICO_PATH)
    set(PIMORONI_PICO_PATH $ENV{PIMORONI_PICO_PATH})
endif ()

include(${PIMORONI_PICO_PATH}/pimoroni_pico_import.cmake)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(Pimoroni_display_2_0_handling C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_library(Pimoroni_display_2_0_handling)

target_include_directories(Pimoroni_display_2_0_handling PUBLIC include)
target_sources(Pimoroni_display_2_0_handling PRIVATE
        src/DisplayHandler.cpp
)

add_subdirectory(${PIMORONI_PICO_PATH}/common pimoroni/common)
add_subdirectory(${PIMORONI_PICO_PATH}/drivers pimoroni/drivers)
add_subdirectory(${PIMORONI_PICO_PATH}/libraries pimoroni/libraries)

target_link_libraries(Pimoroni_display_2_0_handling pico_stdlib hardware_spi hardware_pwm hardware_dma rgbled button pico_display_2 st7789 pico_graphics)
