#pragma once

#include "ImageDefinitions.hpp"

#include <libraries/pico_display_2/pico_display_2.hpp>
#include <drivers/st7789/st7789.hpp>
#include <libraries/pico_graphics/pico_graphics.hpp>
#include <drivers/rgbled/rgbled.hpp>
#include <drivers/button/button.hpp>

#include <memory>

class DisplayHandler {
 public:
  static DisplayHandler* getInstance();
  void initialize();
  void beginRenderFrame();
  void endRenderFrame();
  void placeTextAtPosition(std::string text, pimoroni::Point position, bitmap::font_t font = font8, PenColor text_color = PenColor::WHITE);
  void placeImageForRender(const Image &image, std::pair<int32_t, int32_t> position);
  void placeAnimatedImageForRender(const AnimatedImage &animated_image, std::pair<int32_t, int32_t> position, unsigned int frame_number);
  bool isButtonAPressed();
  bool isButtonBPressed();
  bool isButtonXPressed();
  bool isButtonYPressed();
  void disableLED();
  void setLEDColorHSV(float hue, float saturation, float value);
 private:
  DisplayHandler() = default;
  void from_hsv(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b);

  static inline DisplayHandler* instance {nullptr};

  // HARDWARE SETUP
  pimoroni::ST7789 st7789 {320, 240, pimoroni::ROTATE_0, false, get_spi_pins(pimoroni::BG_SPI_FRONT)};
  pimoroni::PicoGraphics_PenRGB332 graphics {st7789.width, st7789.height, nullptr};
  pimoroni::RGBLED led{pimoroni::PicoDisplay2::LED_R, pimoroni::PicoDisplay2::LED_G, pimoroni::PicoDisplay2::LED_B};
  pimoroni::Button button_a{pimoroni::PicoDisplay2::A};
  pimoroni::Button button_b{pimoroni::PicoDisplay2::B};
  pimoroni::Button button_x{pimoroni::PicoDisplay2::X};
  pimoroni::Button button_y{pimoroni::PicoDisplay2::Y};
};
