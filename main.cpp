#include <cstring>
#include <cmath>
#include <vector>
#include <cstdlib>

#include "libraries/pico_display_2/pico_display_2.hpp"
#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "drivers/rgbled/rgbled.hpp"
#include "drivers/button/button.hpp"
#include "images.h"
#include "image_frame.h"
#include "object.h"

using namespace pimoroni;

ST7789 st7789(320, 240, ROTATE_0, false, get_spi_pins(BG_SPI_FRONT));
PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, nullptr);

RGBLED led(PicoDisplay2::LED_R, PicoDisplay2::LED_G, PicoDisplay2::LED_B);

Button button_a(PicoDisplay2::A);
Button button_b(PicoDisplay2::B);
Button button_x(PicoDisplay2::X);
Button button_y(PicoDisplay2::Y);

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

int main() {
  // --- ELEMENTS DEFINITIONS AND CREATIONS ----
  //Reward fish definition
  Image_frame reward_fish_frames[1] = {Image_frame(graphics, fish_image, 39)};
  Rect reward_fish_colliders[1] = {Rect(0, 0, 13, 8)};
  Object reward_fish(reward_fish_frames, 1, reward_fish_colliders, 1);

  //Meteorites definitions
  Image_frame meteorite_frames[1] = {Image_frame(graphics, meteorite_image, 65)};
  Rect meteorite_colliders[1] = {Rect(0, 0, 0, 0)};
  Object meteorite_object(meteorite_frames, 1, meteorite_colliders, 1);

  //Star definition
  Image_frame star_frames[1] = {Image_frame(graphics, star_image, 89)};
  Rect star_colliders[1] = {Rect(0, 0, 0, 0)};
  Object star_object(star_frames, 1, star_colliders, 1);

  //Rainbow effect definition
  Image_frame rainbow_frames[2] = {Image_frame(graphics, rainbow_frame_1, 24),
                                   Image_frame(graphics, rainbow_frame_2, 24)};
  Rect rainbow_colliders[1] = {Rect(0, 0, 0, 0)};
  Object rainbow_object(rainbow_frames, 2, rainbow_colliders, 1);

  //Cat definition
  Image_frame cat_frames[6] = {Image_frame(graphics, cat_frame_1, 99),
                               Image_frame(graphics, cat_frame_2, 93),
                               Image_frame(graphics, cat_frame_3, 93),
                               Image_frame(graphics, cat_frame_4, 93),
                               Image_frame(graphics, cat_frame_5, 95),
                               Image_frame(graphics, cat_frame_6, 98)};
  Rect cat_colliders[1] = {Rect(0, 0, 0, 0)};
  Object cat_object(cat_frames, 6, cat_colliders, 1);

  //BG Stars definitions
  Image_frame bg_stars_frames[4]{Image_frame(graphics, bg_star_frame_1, 8),
                                 Image_frame(graphics, bg_star_frame_2, 5),
                                 Image_frame(graphics, bg_star_frame_3, 4),
                                 Image_frame(graphics, bg_star_frame_4, 4)};
  Rect bg_stars_colliders[1]{Rect(0, 0, 0, 0)};
  Object bg_stars[30];
  for (int i = 0; i < 30; ++i) {
    bg_stars[i] = Object(bg_stars_frames, 4, bg_stars_colliders, 1);
  }

  st7789.set_backlight(255);

//  struct pt {
//    float x;
//    float y;
//    uint8_t r;
//    float dx;
//    float dy;
//    uint16_t pen;
//  };

//  std::vector<pt> shapes;
//  for (int i = 0; i < 100; i++) {
//    pt shape;
//    shape.x = rand() % graphics.bounds.w;
//    shape.y = rand() % graphics.bounds.h;
//    shape.r = (rand() % 10) + 3;
//    shape.dx = float(rand() % 255) / 64.0f;
//    shape.dy = float(rand() % 255) / 64.0f;
//    shape.pen = graphics.create_pen(rand() % 255, rand() % 255, rand() % 255);
//    shapes.push_back(shape);
//  }

  Point text_location(160, 120);

  Pen BG = graphics.create_pen(0, 50, 103);
  Pen WHITE = graphics.create_pen(255, 255, 255);

  int moving_fish_test_counter = 0;
  int bg_animation_counter = 0;
  int rainbow_animation_counter = 0;
  int cat_animation_counter = 0;
  while (true) {
//    if (button_a.raw()) text_location.x -= 1;
//    if (button_b.raw()) text_location.x += 1;
//
//    if (button_x.raw()) text_location.y -= 1;
//    if (button_y.raw()) text_location.y += 1;

    graphics.set_pen(BG);
    graphics.clear();

    //Background Stars
    //Animation change
    bg_animation_counter = int(millis() / 500) % 4;
    int sign_x = 1;
    int sign_y = 1;
    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 5; ++j) {
        bg_stars[4 * i + j].render(graphics,
                                   20 + (50 * i) + (10 * sign_x * sign_y),
                                   20 + (45 * j) + (5 * sign_y),
                                   bg_animation_counter);
        sign_y *= -1;
        bg_animation_counter = (bg_animation_counter + 1) % 4;
      }
      sign_x *= -1;
    }


//    for (auto &shape : shapes) {
//      shape.x += shape.dx;
//      shape.y += shape.dy;
//      if ((shape.x - shape.r) < 0) {
//        shape.dx *= -1;
//        shape.x = shape.r;
//      }
//      if ((shape.x + shape.r) >= graphics.bounds.w) {
//        shape.dx *= -1;
//        shape.x = graphics.bounds.w - shape.r;
//      }
//      if ((shape.y - shape.r) < 0) {
//        shape.dy *= -1;
//        shape.y = shape.r;
//      }
//      if ((shape.y + shape.r) >= graphics.bounds.h) {
//        shape.dy *= -1;
//        shape.y = graphics.bounds.h - shape.r;
//      }
//
//      graphics.set_pen(shape.pen);
//      graphics.circle(Point(shape.x, shape.y), shape.r);
//
//    }

    // Since HSV takes a float from 0.0 to 1.0 indicating hue,
    // then we can divide millis by the number of milliseconds
    // we want a full colour cycle to take. 5000 = 5 sec.
    uint8_t r = 0, g = 0, b = 0;
    from_hsv((float) millis() / 5000.0f, 1.0f, 0.5f + sinf(millis() / 100.0f / 3.14159f) * 0.5f, r, g, b);
    led.set_rgb(r, g, b);

    reward_fish.render(graphics, 280 - moving_fish_test_counter, 20, 0);
    moving_fish_test_counter++;
    moving_fish_test_counter++;
    if (moving_fish_test_counter > 280) moving_fish_test_counter = 0;

    meteorite_object.render(graphics, 60, 60, 0);

    star_object.render(graphics, 120, 120, 0);

    rainbow_object.render(graphics, 120, 180, rainbow_animation_counter);
    rainbow_animation_counter = int(millis() / 100) % 2;

    cat_animation_counter = int(millis() / 100) % 6;

    cat_object.render(graphics, 180, 180, cat_animation_counter);

//    graphics.set_pen(WHITE);
//    graphics.text("Hello World", text_location, 320);

    // update screen
    st7789.update(&graphics);
  }

  return 0;
}