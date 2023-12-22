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

target_include_directories(No_Internet_cat PUBLIC include assets)
target_sources(No_Internet_cat PRIVATE
        src/Game.cpp
        src/game_object/GameObject.cpp
        src/game_object/GameObjectAssets.cpp
        src/game_object/GameObjectFactory.cpp
        src/AnimationCounter.cpp
        src/RainbowModeHandler.cpp
        src/ObjectSpawner.cpp
        src/BackgroundStarsHandler.cpp
        src/UIHandler.cpp
)

add_subdirectory(libs)

pico_enable_stdio_usb(No_Internet_cat 1)
pico_enable_stdio_uart(No_Internet_cat 1)

pico_add_extra_outputs(No_Internet_cat)

target_link_libraries(No_Internet_cat Pimoroni_display_2_0_handling)