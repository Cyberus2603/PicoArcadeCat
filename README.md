# Pico Arcade Cat 

This is a game for raspberry pi pico microcontroller and Pimoroni pico display pack 2.0.

Short video presenting the game can be found below:

[![Short video presentation](https://github.com/Cyberus2603/PicoArcadeCat/blob/master/github_images/img1.jpg)](https://www.youtube.com/watch?v=1AZOfAV31o0)

# Setup

1. Install [Pico-SDK](https://github.com/raspberrypi/pico-sdk) and [Pimoroni-Pico](https://github.com/pimoroni/pimoroni-pico):

2. Set up PATH variables:

- PICO_SDK_PATH pointing to Pico-SDK
- PIMORONI_PICO_PATH pointing to Pimoroni-Pico

#### If you don't want to change PATH variables you can setup SDK's by changing CMakeLists.txt lines 11 and/or 15

3. Build project with cmake (either Debug or Release)

4. Upload it to pico via Drag-and-Drop (.uf2 file) or via openocd based debugger (.elf file)

5. Have fun !