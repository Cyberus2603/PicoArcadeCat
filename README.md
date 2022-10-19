# Pico Arcade Cat 

This is a game for raspberry pi pico microcontroller and pimoroni pico display pack 2.0.

![](https://github.com/Cyberus2603/PicoArcadeCat/blob/master/img1.jpg)

# Setup

1. Install [Pico-SDK](https://github.com/raspberrypi/pico-sdk) and [Pimoroni-Pico](https://github.com/pimoroni/pimoroni-pico) in paths:

- Pico-SDK in ~/pico/pico-sdk
- Pimoroni-SDK in ~/pico/pimoroni-pico

#### If you have them installed in different location change PATH variables in CMakeLists.txt in line 3 and 4

2. Build project with cmake

3. Upload it to pico via Drag-and-Drop (.uf2 file) or openocd (.elf file)

4. Enjoy