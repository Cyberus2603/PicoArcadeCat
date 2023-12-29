#include <cmath>
#include <vector>
#include <cstdlib>
#include <string>

#include "PimoroniDisplayHandler.hpp"
#include "UI.hpp"

#include "Images.hpp"
#include "object.hpp"

// --- ELEMENTS DEFINITIONS AND CREATIONS ----

//Reward fish definition
pimoroni::Rect reward_fish_collider {0, 0, 13 * PIXEL_SIZE, 8 * PIXEL_SIZE};

//Meteorites definitions
pimoroni::Rect meteorite_collider {0, 0, 23 * PIXEL_SIZE, 21 * PIXEL_SIZE};

//Star definition
pimoroni::Rect star_collider {0, 0, 18 * PIXEL_SIZE, 18 * PIXEL_SIZE};

//Rainbow effect definition
pimoroni::Rect rainbow_collider {0, 0, 0, 0};

//Cat definition
pimoroni::Rect cat_collider {0, 0, 32 * PIXEL_SIZE, 18 * PIXEL_SIZE};

// Background Stars definitions
pimoroni::Rect bg_stars_collider {0, 0, 0, 0};

// --- GAME LOGIC ELEMENTS ---

// Game state definition
enum GameState { game, title, paused, game_over };
enum GameState game_state = title;

// Animation timer
unsigned int animation_counter = 0;

bool animation_tick_function(struct repeating_timer *t) {
  ++animation_counter;
  return true;
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
  return true;
}

// Object spawner timer
#define MIN_SPAWN_TIME 1500
#define MAX_SPAWN_TIME 5000
alarm_id_t spawn_object_alarm;
std::vector<Object> spawned_objects = std::vector<Object>();
int64_t object_spawner_function(alarm_id_t id, void *user_data) {
  int obj_type_to_spawn = rand() % 100;

  if (obj_type_to_spawn < 30) {
    spawned_objects.emplace_back(&FISH_VISUAL_ASSET, reward_fish_collider, fish, 300, (20 + rand()) % 200);
  } else if (obj_type_to_spawn < 90) {
    spawned_objects.emplace_back(&METEORITE_VISUAL_ASSET, meteorite_collider, meteorite, 300, (20 + rand()) % 200);
  } else {
    spawned_objects.emplace_back(&STAR_VISUAL_ASSET, star_collider, rainbow_star, 300, (20 + rand()) % 200);
  }
  cancel_alarm(spawn_object_alarm);
  spawn_object_alarm = add_alarm_in_ms((MIN_SPAWN_TIME + rand()) % MAX_SPAWN_TIME, object_spawner_function, NULL, false);
  return 0;
}

//Gameplay functions
bool do_dynamic_objects_motion_tick(struct repeating_timer *t) {
  uint8_t objects_speed = 2 + int((unsigned int)t->user_data / 4);
  std::vector<int> tmp_ids = std::vector<int>();
  for (int i = 0; i < spawned_objects.size(); ++i) {
    spawned_objects[i].set_pos(spawned_objects[i].get_position_x() - objects_speed, spawned_objects[i].get_position_y());
    if (spawned_objects[i].get_position_x() < 20) tmp_ids.emplace_back(i);
  }
  for (int i = 0; i < tmp_ids.size(); ++i) {
    spawned_objects.erase(spawned_objects.begin() + tmp_ids[i]);
  }
  return true;
}

void render_dynamic_objects() {
  for (int i = 0; i < spawned_objects.size(); ++i) {
    spawned_objects[i].render( spawned_objects[i].get_position_x(), spawned_objects[i].get_position_y(), animation_counter);
  }
}

int check_collisions_with_objects(Object &player) {
  for (int i = 0; i < spawned_objects.size(); ++i) {
    if (player.check_collision(spawned_objects[i])) {
      return i;
    }
  }
  return INT32_MAX;
}

int main() {
  // Non-dynamic objects setup
  Object cat_object(&CAT_VISUAL_ASSET, cat_collider, cat, 130, 140);

  Object rainbow_object(&RAINBOW_VISUAL_ASSET, rainbow_collider, rainbow, 110, 140);

  Object bg_stars[30];
  for (int i = 0; i < 30; ++i) {
    bg_stars[i] = Object(&BACKGROUND_STAR_VISUAL_ASSET, bg_stars_collider, background_star, 0, 0);
  }

  // --- DISPLAY, LED AND UI ELEMENTS SETUP ---
  initializeDisplay();

  //UI Texts Positions
  pimoroni::Point in_game_score_text_location(130, 10);
  pimoroni::Point rainbow_time_text_location(90, 220);
  pimoroni::Point menu_title_text_location(120, 80);
  pimoroni::Point menu_option_1_text_location(120, 110);
  pimoroni::Point menu_option_2_text_location(120, 130);

  // Animation ticks setup
  struct repeating_timer animation_timer;
  add_repeating_timer_ms(-100, animation_tick_function, NULL, &animation_timer);

  // Extra variables and timers
  struct repeating_timer rainbow_mode_timer;
  unsigned int score = 0;
  std::string score_text;
  std::string rainbow_mode_time_text;

  struct repeating_timer motion_timer;
  add_repeating_timer_ms(-20, do_dynamic_objects_motion_tick, (void*) score, &motion_timer);

  while (true) {
    // Display cleanup
    clearDisplay();

    //Background Stars animation change
    int sign_x = 1;
    int sign_y = 1;
    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 5; ++j) {
        bg_stars[4 * i + j].render(
                                   20 + (50 * i) + (10 * sign_x * sign_y),
                                   20 + (45 * j) + (5 * sign_y),
                                   animation_counter + (4 * i + j));
        sign_y *= -1;
      }
      sign_x *= -1;
    }

    // Main program logic
    switch (game_state) {
      case title: {
        // Input handling
        if (buttonXClicked()) {
          game_state = game;
          cat_object.set_pos(20, 110);

          spawn_object_alarm = add_alarm_in_ms((MIN_SPAWN_TIME + rand()) % MAX_SPAWN_TIME, object_spawner_function, NULL, false);
          break;
        }
        cat_object.set_pos(130, 140);
        rainbow_object.set_pos(110, 140);

        // UI rendering
        disableLED();

        renderTitleText();

        rainbow_object.render(rainbow_object.get_position_x(), rainbow_object.get_position_y(), animation_counter);
        cat_object.render(cat_object.get_position_x(), cat_object.get_position_y(), animation_counter);
        break;
      }
      case game: {
        // Input handling
        if (buttonXClicked()) {
          game_state = paused;

          if (spawn_object_alarm) {
            cancel_alarm(spawn_object_alarm);
            spawn_object_alarm = 0;
          }
          break;
        }
        if (buttonAPressed()) {
          cat_object.set_pos(cat_object.get_position_x(),
                             (cat_object.get_position_y() > 10) ? (cat_object.get_position_y() - 2)
                                                                : cat_object.get_position_y());
        }
        if (buttonBPressed()) {
          cat_object.set_pos(cat_object.get_position_x(),
                             (cat_object.get_position_y() < 190) ? (cat_object.get_position_y() + 2)
                                                                 : cat_object.get_position_y());
        }

        // --- GAME LOGIC ---

        // Rainbow mode handling
        if (rainbow_mode) {
          rainbow_object.set_pos(cat_object.get_position_x() - 20, cat_object.get_position_y());
          rainbow_object.render(rainbow_object.get_position_x(), rainbow_object.get_position_y(), animation_counter);

          renderRainbowModeText(rainbow_time_value);

          setLEDColorHSV((float) pimoroni::millis() / 5000.0f, 1.0f, 0.5f + sinf(pimoroni::millis() / 100.0f / 3.14159f) * 0.5f);
        } else {
          disableLED();
        }

        cat_object.render(cat_object.get_position_x(), cat_object.get_position_y(), animation_counter);

        render_dynamic_objects();

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
              if (rainbow_mode) cancel_repeating_timer(&rainbow_mode_timer);
              rainbow_mode = true;
              rainbow_time_value = RAINBOW_TIME_LENGTH;
              add_repeating_timer_ms(-1000, rainbow_time_tick_function, NULL, &rainbow_mode_timer);
              spawned_objects.erase(spawned_objects.begin() + collided_with);
              break;
            }
            default: {break;}
          }
        }

        // UI Text elements
        renderScoreText(score);

        break;
      }
      case paused: {
        // Input handling
        if (buttonXClicked()) {
          game_state = game;

          spawn_object_alarm = add_alarm_in_ms((MIN_SPAWN_TIME + rand()) % MAX_SPAWN_TIME, object_spawner_function, NULL, false);
          break;
        }

        // UI rendering
        renderPausedGameText(score);

        break;
      }
      case game_over: {
        // Input handling
        if (buttonXClicked()) {
          game_state = title;
          break;
        }

        // Timers cleanup
        if (spawn_object_alarm) {
          cancel_alarm(spawn_object_alarm);
          spawn_object_alarm = 0;
        }

        if (rainbow_mode) {
          cancel_repeating_timer(&rainbow_mode_timer);
          rainbow_time_value = RAINBOW_TIME_LENGTH;
        }

        // UI rendering
        disableLED();

        renderGameOverText(score);

        break;
      }
    }

    // update screen
    updateDisplay();
  }
}