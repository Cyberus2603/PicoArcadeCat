cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico)

include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

project(No_Internet_cat C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(No_Internet_cat main.cpp object.h object.cpp image_frame.h image_frame.cpp images.h)

add_subdirectory(${PIMORONI_PICO_PATH}/common pimoroni/common)
add_subdirectory(${PIMORONI_PICO_PATH}/drivers pimoroni/drivers)
add_subdirectory(${PIMORONI_PICO_PATH}/libraries pimoroni/libraries)

pico_enable_stdio_usb(No_Internet_cat 1)
pico_enable_stdio_uart(No_Internet_cat 1)

pico_add_extra_outputs(No_Internet_cat)

target_link_libraries(No_Internet_cat pico_stdlib hardware_spi hardware_pwm hardware_dma rgbled button pico_display_2 st7789 pico_graphics)