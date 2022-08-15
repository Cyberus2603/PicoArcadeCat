cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH ~/pico/pico-sdk)
set(PIMORONI_PICO_PATH ~/pico/pimoroni-pico)
include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

project(No_Internet_cat C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_executable(No_Internet_cat main.cpp object.h object.cpp image_frame.h image_frame.cpp images.h)

include(${PIMORONI_PICO_PATH}/drivers/st7789/st7789.cmake)
include(${PIMORONI_PICO_PATH}/drivers/rgbled/rgbled.cmake)
include(${PIMORONI_PICO_PATH}/drivers/button/button.cmake)
include(${PIMORONI_PICO_PATH}/libraries/pico_graphics/pico_graphics.cmake)
include(${PIMORONI_PICO_PATH}/libraries/bitmap_fonts/bitmap_fonts.cmake)
include(${PIMORONI_PICO_PATH}/libraries/hershey_fonts/hershey_fonts.cmake)
include(${PIMORONI_PICO_PATH}/libraries/pico_display_2/pico_display_2.cmake)
include(${PIMORONI_PICO_PATH}/common/pimoroni_bus.cmake)

pico_enable_stdio_usb(No_Internet_cat 1)
pico_enable_stdio_uart(No_Internet_cat 1)

pico_add_extra_outputs(No_Internet_cat)

target_link_libraries(No_Internet_cat pico_stdlib hardware_spi hardware_pwm hardware_pio hardware_dma rgbled button pico_display_2 st7789 pico_graphics)