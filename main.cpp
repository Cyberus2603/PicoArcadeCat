#include <cstring>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <string>

#include "libraries/pico_display_2/pico_display_2.hpp"
#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "drivers/rgbled/rgbled.hpp"
#include "drivers/button/button.hpp"
#include "images.h"
#include "image_frame.h"
#include "object.h"
#include "pico/stdio.h"

using namespace pimoroni;

ST7789 st7789(320, 240, ROTATE_0, false, get_spi_pins(BG_SPI_FRONT));
PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, nullptr);

RGBLED led(PicoDisplay2::LED_R, PicoDisplay2::LED_G, PicoDisplay2::LED_B);

Button button_a(PicoDisplay2::A);
Button button_b(PicoDisplay2::B);
Button button_x(PicoDisplay2::X);
Button button_y(PicoDisplay2::Y);

enum GameState { game, title, paused, game_over };

unsigned int animation_counter = 30;

bool animation_tick_function(struct repeating_timer *t) {
  animation_counter++;
}

uint8_t rainbow_time_value = 0;
bool rainbow_mode = false;

bool rainbow_time_tick_function(struct repeating_timer *t) {
  rainbow_time_value--;
  if (rainbow_time_value == 0) {
    rainbow_time_value = 30;
    rainbow_mode = false;
  }
}

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
  enum GameState game_state = game;
  //Reward fish definition
  Image_frame reward_fish_frames[1] = {Image_frame(graphics, fish_image, 39)};
  Rect reward_fish_colliders[1] = {Rect(0, 0, 13 * PIXEL_SIZE, 8 * PIXEL_SIZE)};
  Object reward_fish(reward_fish_frames, 1, reward_fish_colliders, 1, 0, 0);

  //Meteorites definitions
  Image_frame meteorite_frames[1] = {Image_frame(graphics, meteorite_image, 65)};
  Rect meteorite_colliders[1] = {Rect(0, 0, 23 * PIXEL_SIZE, 21 * PIXEL_SIZE)};
  Object meteorite_object(meteorite_frames, 1, meteorite_colliders, 1, 0, 0);

  //Star definition
  Image_frame star_frames[1] = {Image_frame(graphics, star_image, 89)};
  Rect star_colliders[1] = {Rect(0, 0, 18 * PIXEL_SIZE, 18 * PIXEL_SIZE)};
  Object star_object(star_frames, 1, star_colliders, 1, 0, 0);

  //Rainbow effect definition
  Image_frame rainbow_frames[2] = {Image_frame(graphics, rainbow_frame_1, 24),
                                   Image_frame(graphics, rainbow_frame_2, 24)};
  Rect rainbow_colliders[1] = {Rect(0, 0, 0, 0)};
  Object rainbow_object(rainbow_frames, 2, rainbow_colliders, 1, 0, 0);

  //Cat definition
  Image_frame cat_frames[6] = {Image_frame(graphics, cat_frame_1, 99),
                               Image_frame(graphics, cat_frame_2, 93),
                               Image_frame(graphics, cat_frame_3, 93),
                               Image_frame(graphics, cat_frame_4, 93),
                               Image_frame(graphics, cat_frame_5, 95),
                               Image_frame(graphics, cat_frame_6, 98)};
  Rect cat_colliders[1] = {Rect(0, 0, 32 * PIXEL_SIZE, 18 * PIXEL_SIZE)};
  Object cat_object(cat_frames, 6, cat_colliders, 1, 10, 110);

  //background_color Stars definitions
  Image_frame bg_stars_frames[4]{Image_frame(graphics, bg_star_frame_1, 8),
                                 Image_frame(graphics, bg_star_frame_2, 5),
                                 Image_frame(graphics, bg_star_frame_3, 4),
                                 Image_frame(graphics, bg_star_frame_4, 4)};
  Rect bg_stars_colliders[1]{Rect(0, 0, 0, 0)};
  Object bg_stars[30];
  for (int i = 0; i < 30; ++i) {
    bg_stars[i] = Object(bg_stars_frames, 4, bg_stars_colliders, 1, 0, 0);
  }

  // --- DISPLAY, LED AND UI ELEMENTS SETUP ---
  st7789.set_backlight(255);
  Pen background_color = graphics.create_pen(0, 50, 103);
  Pen text_color = graphics.create_pen(255, 255, 255);
  // RGB LED Variables
  uint8_t led_r = 0, led_g = 0, led_b = 0;

  //UI Texts Positions
  Point in_game_score_text_location(130, 10);
  Point rainbow_time_text_location(60, 220);
  Point menu_title_text_location(120, 80);
  Point menu_option_1_text_location(120, 110);
  Point menu_option_2_text_location(120, 130);

  // Animation ticks setup
  struct repeating_timer animation_timer;
  add_repeating_timer_ms(-100, animation_tick_function, NULL, &animation_timer);

  int moving_fish_test_counter = 0;

  // Extra variables and timers
  struct repeating_timer rainbow_mode_timer;
  unsigned int score = 0;
  std::string score_text;
  std::string rainbow_mode_time_text;

  while (true) {

//    if (button_x.raw()) in_game_score_text_location.y -= 1;
//    if (button_y.raw()) in_game_score_text_location.y += 1;

    // Display cleanup
    graphics.set_pen(background_color);
    graphics.clear();

    //Background Stars animation change
    int sign_x = 1;
    int sign_y = 1;
    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 5; ++j) {
        bg_stars[4 * i + j].render(graphics,
                                   20 + (50 * i) + (10 * sign_x * sign_y),
                                   20 + (45 * j) + (5 * sign_y),
                                   int(animation_counter / 5) % bg_stars[4 * i + j].get_frames_size());
        sign_y *= -1;
      }
      sign_x *= -1;
    }

    // Main program logic
    switch (game_state) {
      case title: {

        break;
      }
      case game: {
        if (button_a.raw()) {
          cat_object.set_pos(cat_object.get_position_x(),
                             (cat_object.get_position_y() > 10) ? (cat_object.get_position_y() - 2)
                                                                : cat_object.get_position_y());
        }
        if (button_b.raw()) {
          cat_object.set_pos(cat_object.get_position_x(),
                             (cat_object.get_position_y() < 230) ? (cat_object.get_position_y() + 2)
                                                                 : cat_object.get_position_y());
        }

        cat_object.render(graphics, cat_object.get_position_x(), cat_object.get_position_y(), animation_counter % cat_object.get_frames_size());

        reward_fish.set_pos(280 - moving_fish_test_counter, 20);
        reward_fish.render(graphics, reward_fish.get_position_x(), reward_fish.get_position_y() , 0);
        moving_fish_test_counter++;
        if (moving_fish_test_counter > 280) moving_fish_test_counter = 0;

        if (cat_object.check_collision(reward_fish)) score++;

        break;
      }
      case paused: {

        break;
      }
      case game_over: {

        break;
      }
    }

    from_hsv((float) millis() / 5000.0f, 1.0f, 0.5f + sinf(millis() / 100.0f / 3.14159f) * 0.5f, led_r, led_g, led_b);
    led.set_rgb(led_r, led_g, led_b);

    reward_fish.render(graphics, 280 - moving_fish_test_counter, 20, 0);
    moving_fish_test_counter++;
    moving_fish_test_counter++;
    if (moving_fish_test_counter > 280) moving_fish_test_counter = 0;

//    meteorite_object.render(graphics, 60, 60, 0);
//
//    star_object.render(graphics, 120, 120, 0);
//
//    rainbow_object.render(graphics, 120, 180, animation_counter % rainbow_object.get_frames_size());

    // UI Text elements
    score_text = "Score: " + std::to_string(score);
    graphics.set_pen(text_color);
    graphics.set_font(&font6);
    graphics.text(score_text, in_game_score_text_location, 320);

    rainbow_mode_time_text = "Rainbow time left: " + std::to_string(rainbow_time_value);
    graphics.set_pen(text_color);
    graphics.set_font(&font6);
    graphics.text(rainbow_mode_time_text, rainbow_time_text_location, 320);

//    graphics.set_pen(text_color);
//    graphics.set_font(&font14_outline);
//    graphics.text("GAME OVER", menu_title_text_location, 320);
//
//    graphics.set_pen(text_color);
//    graphics.set_font(&font8);
//    graphics.text(score_text, menu_option_1_text_location, 320);
//
//    graphics.set_pen(text_color);
//    graphics.set_font(&font8);
//    graphics.text("Press X to restart", menu_option_2_text_location, 320);


    // update screen
    st7789.update(&graphics);
  }

  return 0;
}