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

project(No_Internet_cat C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(No_Internet_cat main.cpp)
target_include_directories(No_Internet_cat PRIVATE include assets)
target_sources(No_Internet_cat PRIVATE
        src/object.cpp
)

add_subdirectory(${PIMORONI_PICO_PATH}/common pimoroni/common)
add_subdirectory(${PIMORONI_PICO_PATH}/drivers pimoroni/drivers)
add_subdirectory(${PIMORONI_PICO_PATH}/libraries pimoroni/libraries)

pico_enable_stdio_usb(No_Internet_cat 1)
pico_enable_stdio_uart(No_Internet_cat 1)

pico_add_extra_outputs(No_Internet_cat)

target_link_libraries(No_Internet_cat pico_stdlib hardware_spi hardware_pwm hardware_dma rgbled button pico_display_2 st7789 pico_graphics)