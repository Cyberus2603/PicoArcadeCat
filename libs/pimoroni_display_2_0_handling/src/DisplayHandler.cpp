#include "DisplayHandler.hpp"

#include "ColorDefinitons.hpp"

DisplayHandler* DisplayHandler::getInstance() {
  if (instance == nullptr) {
    instance = new DisplayHandler();
  }
  return instance;
}

void DisplayHandler::initialize() {
  st7789.set_backlight(255);
}

void DisplayHandler::beginRenderFrame() {
  auto background_color = COLORS.at(PenColor::DEEP_BLUE);
  graphics.set_pen(background_color.red, background_color.green, background_color.blue);
  graphics.clear();
}

void DisplayHandler::endRenderFrame() {
  st7789.update(&graphics);
}

void DisplayHandler::placeTextAtPosition(std::string text,
                                         pimoroni::Point position,
                                         bitmap::font_t font,
                                         PenColor text_color) {
  auto pen_color = COLORS.at(text_color);
  graphics.set_pen(pen_color.red, pen_color.green, pen_color.blue);
  graphics.set_font(&font);
  graphics.text(text.c_str(), position, 320);
}


void DisplayHandler::placeImageForRender(const Image &image, std::pair<int32_t, int32_t> position) {
  for (auto image_fragment : image) {
    auto pen_color = COLORS.at(image_fragment.color);
    graphics.set_pen(pen_color.red, pen_color.green, pen_color.blue);
    pimoroni::Rect colored_area {image_fragment.start_point.first + position.first,
                                 image_fragment.start_point.second + position.second,
                                 image_fragment.width, image_fragment.height};
    graphics.rectangle(colored_area);
  }
}
void DisplayHandler::placeAnimatedImageForRender(const AnimatedImage &animated_image,
                                                 std::pair<int32_t, int32_t> position,
                                                 unsigned int frame_number) {
  DisplayHandler::placeImageForRender(animated_image.at(frame_number), position);
}

//TODO: Refactor
// HSV Conversion expects float inputs in the range of 0.00-1.00 for each channel
// Outputs are rgb in the range 0-255 for each channel
void DisplayHandler::from_hsv(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
  float i = floor(h * 6.0f);
  float f = h * 6.0f - i;
  v *= 255.0f;
  uint8_t p = v * (1.0f - s);
  uint8_t q = v * (1.0f - f * s);
  uint8_t t = v * (1.0f - (1.0f - f) * s);

  switch (int(i) % 6) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    case 5:
      r = v;
      g = p;
      b = q;
      break;
  }
}

bool DisplayHandler::isButtonAPressed() {
  return button_a.raw();
}

bool DisplayHandler::isButtonBPressed() {
  return button_b.raw();
}

bool DisplayHandler::isButtonXPressed() {
  return button_x.raw();
}

bool DisplayHandler::isButtonYPressed() {
  return button_y.raw();
}

void DisplayHandler::disableLED() {
  led.set_rgb(0, 0, 0);
}

//TODO: Refactor
void DisplayHandler::setLEDColorHSV(float hue, float saturation, float value) {
  uint8_t led_r = 0, led_g = 0, led_b = 0;
  from_hsv(hue, saturation, value, led_r, led_g, led_b);
  led.set_rgb(led_r, led_g, led_b);
}




