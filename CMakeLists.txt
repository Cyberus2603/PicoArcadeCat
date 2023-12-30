cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

set(CMAKE_C_FLAGS_DEBUG "-Og -g -gdwarf-3 -gstrict-dwarf -gsplit-dwarf")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g -gdwarf-3 -gstrict-dwarf -gsplit-dwarf")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (NOT PICO_SDK_PATH)
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
endif ()

if (NOT PIMORONI_PICO_PATH)
    set(PIMORONI_PICO_PATH $ENV{PIMORONI_PICO_PATH})
endif ()

include(${PIMORONI_PICO_PATH}/pimoroni_pico_import.cmake)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(Pico_Cat C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(Pico_Cat main.cpp)
target_include_directories(Pico_Cat PRIVATE include)
target_sources(Pico_Cat PRIVATE
        src/assets/Images.cpp
        src/assets/GameObjectsPrototypes.cpp
        src/game_internals/AnimationTimer.cpp
        src/game_internals/ObjectMotion.cpp
        src/game_internals/RainbowMode.cpp
        src/game_internals/ObjectSpawner.cpp
        src/game_internals/Object.cpp
        src/game_internals/BackgroundStars.cpp
        src/game_internals/UI.cpp
        src/pimoroni_display/PimoroniDisplayHandler.cpp
)

add_subdirectory(${PIMORONI_PICO_PATH}/common pimoroni/common)
add_subdirectory(${PIMORONI_PICO_PATH}/drivers pimoroni/drivers)
add_subdirectory(${PIMORONI_PICO_PATH}/libraries pimoroni/libraries)

pico_enable_stdio_usb(Pico_Cat 1)
pico_enable_stdio_uart(Pico_Cat 1)

pico_add_extra_outputs(Pico_Cat)

target_link_libraries(Pico_Cat pico_stdlib hardware_spi hardware_pwm hardware_dma rgbled button pico_display_2 st7789 pico_graphics)