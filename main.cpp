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

using namespace pimoroni;

// HARDWARE SETUP
ST7789 st7789(320, 240, ROTATE_0, false, get_spi_pins(BG_SPI_FRONT));
PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, nullptr);

RGBLED led(PicoDisplay2::LED_R, PicoDisplay2::LED_G, PicoDisplay2::LED_B);

Button button_a(PicoDisplay2::A);
Button button_b(PicoDisplay2::B);
Button button_x(PicoDisplay2::X);
Button button_y(PicoDisplay2::Y);

// --- ELEMENTS DEFINITIONS AND CREATIONS ----

//Reward fish definition
Image_frame reward_fish_frames[1] = {Image_frame(graphics, fish_image, 39)};
Rect reward_fish_colliders[1] = {Rect(0, 0, 13 * PIXEL_SIZE, 8 * PIXEL_SIZE)};

//Meteorites definitions
Image_frame meteorite_frames[1] = {Image_frame(graphics, meteorite_image, 65)};
Rect meteorite_colliders[1] = {Rect(0, 0, 23 * PIXEL_SIZE, 21 * PIXEL_SIZE)};

//Star definition
Image_frame star_frames[1] = {Image_frame(graphics, star_image, 89)};
Rect star_colliders[1] = {Rect(0, 0, 18 * PIXEL_SIZE, 18 * PIXEL_SIZE)};

//Rainbow effect definition
Image_frame rainbow_frames[2] = {Image_frame(graphics, rainbow_frame_1, 24),
                                 Image_frame(graphics, rainbow_frame_2, 24)};
Rect rainbow_colliders[1] = {Rect(0, 0, 0, 0)};

//Cat definition
Image_frame cat_frames[6] = {Image_frame(graphics, cat_frame_1, 99),
                             Image_frame(graphics, cat_frame_2, 93),
                             Image_frame(graphics, cat_frame_3, 93),
                             Image_frame(graphics, cat_frame_4, 93),
                             Image_frame(graphics, cat_frame_5, 95),
                             Image_frame(graphics, cat_frame_6, 98)};
Rect cat_colliders[1] = {Rect(0, 0, 32 * PIXEL_SIZE, 18 * PIXEL_SIZE)};

// Background Stars definitions
Image_frame bg_stars_frames[4]{Image_frame(graphics, bg_star_frame_1, 8),
                               Image_frame(graphics, bg_star_frame_2, 5),
                               Image_frame(graphics, bg_star_frame_3, 4),
                               Image_frame(graphics, bg_star_frame_4, 4)};
Rect bg_stars_colliders[1]{Rect(0, 0, 0, 0)};

// --- GAME LOGIC ELEMENTS ---

// Game state definition
enum GameState { game, title, paused, game_over };
enum GameState game_state = title;

// Animation timer
unsigned int animation_counter = 0;

bool animation_tick_function(struct repeating_timer *t) {
  animation_counter++;
}

// Rainbow timer
#define RAINBOW_TIME_LENGTH 20
uint8_t rainbow_time_value = RAINBOW_TIME_LENGTH;
bool rainbow_mode = false;

bool rainbow_time_tick_function(struct repeating_timer *t) {
  if (game_state == paused) return true;
  rainbow_time_value--;
  if (rainbow_time_value == 0) {
    rainbow_time_value = RAINBOW_TIME_LENGTH;
    rainbow_mode = false;
  }
}

// Object spawner timer
#define MIN_SPAWN_TIME 1500
#define MAX_SPAWN_TIME 5000
alarm_id_t spawn_object_alarm;
std::vector<Object> spawned_objects = std::vector<Object>();
int64_t object_spawner_function(alarm_id_t id, void *user_data) {
  int obj_type_to_spawn = rand() % 100;

  if (obj_type_to_spawn < 30) {
    spawned_objects.emplace_back(Object(reward_fish_frames, 1, reward_fish_colliders, 1, fish,  300, (20 + rand()) % 200));
  } else if (obj_type_to_spawn < 90) {
    spawned_objects.emplace_back(Object(meteorite_frames, 1, meteorite_colliders, 1, meteorite, 300, (20 + rand()) % 200));
  } else {
    spawned_objects.emplace_back(Object(star_frames, 1, star_colliders, 1, rainbow_star, 300, (20 + rand()) % 200));
  }
  cancel_alarm(spawn_object_alarm);
  spawn_object_alarm = add_alarm_in_ms((MIN_SPAWN_TIME + rand()) % MAX_SPAWN_TIME, object_spawner_function, NULL, false);
}

//Gameplay functions
void do_dynamic_objects_motion_tick(unsigned int score) {
  uint8_t objects_speed = 2 + int(score / 4);
  std::vector<int> tmp_ids = std::vector<int>();
  for (int i = 0; i < spawned_objects.size(); ++i) {
    spawned_objects[i].set_pos(spawned_objects[i].get_position_x() - objects_speed, spawned_objects[i].get_position_y());
    if (spawned_objects[i].get_position_x() < 20) tmp_ids.emplace_back(i);
  }
  for (int i = 0; i < tmp_ids.size(); ++i) {
    spawned_objects.erase(spawned_objects.begin() + tmp_ids[i]);
  }
}

void render_dynamic_objects() {
  for (int i = 0; i < spawned_objects.size(); ++i) {
    spawned_objects[i].render(graphics, spawned_objects[i].get_position_x(), spawned_objects[i].get_position_y(), animation_counter % spawned_objects[i].get_frames_size());
  }
}

int check_collisions_with_objects(Object player) {
  for (int i = 0; i < spawned_objects.size(); ++i) {
    if (player.check_collision(spawned_objects[i])) {
      return i;
    }
  }
  return INT32_MAX;
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
  // Non-dynamic objects setup
  Object cat_object(cat_frames, 6, cat_colliders, 1, cat, 130, 140);

  Object rainbow_object(rainbow_frames, 2, rainbow_colliders, 1, rainbow, 110, 140);

  Object bg_stars[30];
  for (int i = 0; i < 30; ++i) {
    bg_stars[i] = Object(bg_stars_frames, 4, bg_stars_colliders, 1, background_star, 0, 0);
  }

  // --- DISPLAY, LED AND UI ELEMENTS SETUP ---
  st7789.set_backlight(255);
  Pen background_color = graphics.create_pen(0, 50, 103);
  Pen text_color = graphics.create_pen(255, 255, 255);

  //UI Texts Positions
  Point in_game_score_text_location(130, 10);
  Point rainbow_time_text_location(90, 220);
  Point menu_title_text_location(120, 80);
  Point menu_option_1_text_location(120, 110);
  Point menu_option_2_text_location(120, 130);

  // Animation ticks setup
  struct repeating_timer animation_timer;
  add_repeating_timer_ms(-100, animation_tick_function, NULL, &animation_timer);

  // Extra variables and timers
  struct repeating_timer rainbow_mode_timer;
  unsigned int score = 0;
  std::string score_text;
  std::string rainbow_mode_time_text;

  while (true) {
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
        // Input handling
        if (button_x.read()) {
          game_state = game;
          cat_object.set_pos(20, 110);

          spawn_object_alarm = add_alarm_in_ms((MIN_SPAWN_TIME + rand()) % MAX_SPAWN_TIME, object_spawner_function, NULL, false);
          break;
        }

        // UI rendering
        led.set_rgb(0, 0, 0);

        menu_title_text_location.x = 100;
        menu_option_1_text_location.x = 85;

        graphics.set_pen(text_color);
        graphics.set_font(&font14_outline);
        graphics.text("PICO CAT", menu_title_text_location, 320);

        graphics.set_pen(text_color);
        graphics.set_font(&font8);
        graphics.text("Press X to start", menu_option_1_text_location, 320);

        rainbow_object.render(graphics, rainbow_object.get_position_x(), rainbow_object.get_position_y(), animation_counter % rainbow_object.get_frames_size());
        cat_object.render(graphics, cat_object.get_position_x(), cat_object.get_position_y(), animation_counter % cat_object.get_frames_size());
        break;
      }
      case game: {
        // Input handling
        if (button_x.read()) {
          game_state = paused;

          if (spawn_object_alarm) {
            cancel_alarm(spawn_object_alarm);
            spawn_object_alarm = 0;
          }
          break;
        }
        if (button_a.raw()) {
          cat_object.set_pos(cat_object.get_position_x(),
                             (cat_object.get_position_y() > 10) ? (cat_object.get_position_y() - 2)
                                                                : cat_object.get_position_y());
        }
        if (button_b.raw()) {
          cat_object.set_pos(cat_object.get_position_x(),
                             (cat_object.get_position_y() < 190) ? (cat_object.get_position_y() + 2)
                                                                 : cat_object.get_position_y());
        }

        // --- GAME LOGIC ---

        // Rainbow mode handling
        if (rainbow_mode) {
          rainbow_object.set_pos(cat_object.get_position_x() - 20, cat_object.get_position_y());
          rainbow_object.render(graphics, rainbow_object.get_position_x(), rainbow_object.get_position_y(), animation_counter % rainbow_object.get_frames_size());

          rainbow_mode_time_text = "Rainbow left: " + std::to_string(rainbow_time_value);
          graphics.set_pen(text_color);
          graphics.set_font(&font6);
          graphics.text(rainbow_mode_time_text, rainbow_time_text_location, 320);

          uint8_t led_r = 0, led_g = 0, led_b = 0;
          from_hsv((float) millis() / 5000.0f, 1.0f, 0.5f + sinf(millis() / 100.0f / 3.14159f) * 0.5f, led_r, led_g, led_b);
          led.set_rgb(led_r, led_g, led_b);
        } else {
          led.set_rgb(0, 0, 0);
        }

        cat_object.render(graphics, cat_object.get_position_x(), cat_object.get_position_y(), animation_counter % cat_object.get_frames_size());

        render_dynamic_objects();
        do_dynamic_objects_motion_tick(score);

        int collided_with = check_collisions_with_objects(cat_object);

        if (collided_with != INT32_MAX) {
          ObjectTypes collided_type = spawned_objects[collided_with].get_object_type();
          switch (collided_type) {
            case fish: {
              score++;
              spawned_objects.erase(spawned_objects.begin() + collided_with);
              break;
            }
            case meteorite: {
              if (rainbow_mode) {
                spawned_objects.erase(spawned_objects.begin() + collided_with);
              } else {
                spawned_objects.clear();
                game_state = game_over;
              }
              break;
            }
            case rainbow_star: {
              rainbow_mode = true;
              add_repeating_timer_ms(-1000, rainbow_time_tick_function, NULL, &rainbow_mode_timer);
              spawned_objects.erase(spawned_objects.begin() + collided_with);
              break;
            }
            default: {break;}
          }
        }

        // UI Text elements
        score_text = "Score: " + std::to_string(score);
        graphics.set_pen(text_color);
        graphics.set_font(&font6);
        graphics.text(score_text, in_game_score_text_location, 320);

        break;
      }
      case paused: {
        // Input handling
        if (button_x.read()) {
          game_state = game;

          spawn_object_alarm = add_alarm_in_ms((MIN_SPAWN_TIME + rand()) % MAX_SPAWN_TIME, object_spawner_function, NULL, false);
          break;
        }

        // UI rendering
        menu_title_text_location.x = 120;
        menu_option_1_text_location.x = 130;
        menu_option_2_text_location.x = 80;

        graphics.set_pen(text_color);
        graphics.set_font(&font14_outline);
        graphics.text("PAUSED", menu_title_text_location, 320);

        graphics.set_pen(text_color);
        graphics.set_font(&font8);
        graphics.text(score_text, menu_option_1_text_location, 320);

        graphics.set_pen(text_color);
        graphics.set_font(&font8);
        graphics.text("Press X to unpause", menu_option_2_text_location, 320);

        break;
      }
      case game_over: {
        // Input handling
        if (button_x.read()) {
          game_state = title;
          break;
        }

        // Timers cleanup
        if (spawn_object_alarm) {
          cancel_alarm(spawn_object_alarm);
          spawn_object_alarm = 0;
        }

        cancel_repeating_timer(&rainbow_mode_timer);
        rainbow_time_value = RAINBOW_TIME_LENGTH;

        // UI rendering
        led.set_rgb(0, 0, 0);

        menu_title_text_location.x = 100;
        menu_option_1_text_location.x = 110;
        menu_option_2_text_location.x = 100;

        graphics.set_pen(text_color);
        graphics.set_font(&font14_outline);
        graphics.text("GAME OVER", menu_title_text_location, 320);

        graphics.set_pen(text_color);
        graphics.set_font(&font8);
        graphics.text(score_text, menu_option_1_text_location, 320);

        graphics.set_pen(text_color);
        graphics.set_font(&font8);
        graphics.text("Press X to restart", menu_option_2_text_location, 320);

        break;
      }
    }

    // update screen
    st7789.update(&graphics);
  }

  return 0;
}