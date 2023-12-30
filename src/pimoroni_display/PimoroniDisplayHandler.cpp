#include "pimoroni_display/PimoroniDisplayHandler.hpp"

#include <st7789.hpp>
#include <pico_display_2.hpp>
#include <rgbled.hpp>
#include <button.hpp>

pimoroni::ST7789 st7789(SCREEN_WIDTH, SCREEN_HEIGHT, pimoroni::ROTATE_0, false, get_spi_pins(pimoroni::BG_SPI_FRONT));
pimoroni::PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, nullptr);
pimoroni::RGBLED led(pimoroni::PicoDisplay2::LED_R, pimoroni::PicoDisplay2::LED_G, pimoroni::PicoDisplay2::LED_B);
pimoroni::Button button_a(pimoroni::PicoDisplay2::A);
pimoroni::Button button_b(pimoroni::PicoDisplay2::B);
pimoroni::Button button_x(pimoroni::PicoDisplay2::X);
pimoroni::Button button_y(pimoroni::PicoDisplay2::Y);

void initializeDisplay() {
  st7789.set_backlight(255);
}

void clearDisplay() {
  ColorParts background_color = Colors::DEEP_BLUE;
  pimoroni::Pen background_color_pen = graphics.create_pen(background_color.red, background_color.green, background_color.blue);
  graphics.set_pen(background_color_pen);
  graphics.clear();
}

void updateDisplay() {
  st7789.update(&graphics);
}

bool buttonXPressed() {
  return button_x.raw();
}

bool buttonXClicked() {
  return button_x.read();
}

bool buttonYPressed() {
  return button_y.raw();
}

bool buttonYClicked() {
  return button_y.read();
}

bool buttonAPressed() {
  return button_a.raw();
}

bool buttonAClicked() {
  return button_a.read();
}

bool buttonBPressed() {
  return button_b.raw();
}

bool buttonBClicked() {
  return button_b.read();
}

void disableLED() {
  led.set_rgb(0, 0, 0);
}

// TODO: Refactor
// HSV Conversion expects float inputs in the range of 0.00-1.00 for each channel
// Outputs are rgb in the range 0-255 for each channel
void from_hsv(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
  float i = floor(h * 6.0f);
  float f = h * 6.0f - i;
  v *= 255.0f;
  uint8_t p = v * (1.0f - s);
  uint8_t q = v * (1.0f - f * s);
  uint8_t t = v * (1.0f - (1.0f - f) * s);

  switch (int(i) % 6) {
    case 0: r = v;
      g = t;
      b = p;
      break;
    case 1: r = q;
      g = v;
      b = p;
      break;
    case 2: r = p;
      g = v;
      b = t;
      break;
    case 3: r = p;
      g = q;
      b = v;
      break;
    case 4: r = t;
      g = p;
      b = v;
      break;
    case 5: r = v;
      g = p;
      b = q;
      break;
  }
}


void setLEDColorHSV(float hue, float saturation, float value) {
  uint8_t led_r = 0, led_g = 0, led_b = 0;
  from_hsv(hue, saturation, value, led_r, led_g, led_b);
  led.set_rgb(led_r, led_g, led_b);
}

void placeTextAtPosition(const std::string& text, const pimoroni::Point &position, const bitmap::font_t& font, const ColorParts& text_color) {
  graphics.set_pen(text_color.red, text_color.green, text_color.blue);
  graphics.set_font(&font);
  graphics.text(text, position, 320);
}

void placeVisualAssetAtPosition(const VisualAsset* visual_asset, const pimoroni::Point &position, unsigned int frame_number) {
  VisualAssetFrame asset_frame = visual_asset->at(frame_number % visual_asset->size());
  for (auto asset_fragment : asset_frame) {
    graphics.set_pen(asset_fragment.color.red, asset_fragment.color.green, asset_fragment.color.blue);
    pimoroni::Rect colored_area {asset_fragment.start_point.first + position.x,
                                 asset_fragment.start_point.second + position.y,
                                 asset_fragment.width, asset_fragment.height};
    graphics.rectangle(colored_area);
  }
}