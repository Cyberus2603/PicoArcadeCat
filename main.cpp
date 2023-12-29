#include "GameSettingAndVariables.hpp"
#include "assets/Images.hpp"
#include "assets/GameObjectsPrototypes.hpp"

#include "PimoroniDisplayHandler.hpp"

#include "BackgroundStars.hpp"
#include "Object.hpp"
#include "UI.hpp"

// --- GAME LOGIC ELEMENTS ---

// Game state definition
enum class GameState { game, title, paused, game_over };
enum GameState game_state {GameState::title};

// Animation timer
unsigned int animation_counter = 0;

bool animation_tick_function(struct repeating_timer *t) {
  ++animation_counter;
  return true;
}

uint8_t rainbow_time_value = RAINBOW_MODE_TIME_LENGTH;
bool rainbow_mode = false;

bool rainbow_time_tick_function(struct repeating_timer *t) {
  if (game_state == GameState::paused) return true;
  rainbow_time_value--;
  if (rainbow_time_value == 0) {
    rainbow_time_value = RAINBOW_MODE_TIME_LENGTH;
    rainbow_mode = false;
  }
  return true;
}

alarm_id_t spawn_object_alarm;
std::vector<Object> spawned_objects;

int64_t object_spawner_function(alarm_id_t id, void *user_data) {
  int obj_type_to_spawn = rand() % 100;

  if (obj_type_to_spawn < 30) {
    Object spawned_fish(FISH_OBJECT_PROTOTYPE);
    spawned_fish.current_position = {300, (20 + rand()) % 200};
    spawned_objects.emplace_back(spawned_fish);
  } else if (obj_type_to_spawn < 90) {
    Object spawned_meteorite(METEORITE_OBJECT_PROTOTYPE);
    spawned_meteorite.current_position = {300, (20 + rand()) % 200};
    spawned_objects.emplace_back(spawned_meteorite);
  } else {
    Object spawned_star(STAR_OBJECT_PROTOTYPE);
    spawned_star.current_position = {300, (20 + rand()) % 200};
    spawned_objects.emplace_back(spawned_star);
  }
  cancel_alarm(spawn_object_alarm);
  spawn_object_alarm = add_alarm_in_ms((MINIMAL_SPAWNER_INTERVAL + rand()) % MAXIMAL_SPAWNER_INTERVAL, object_spawner_function, NULL, false);
  return 0;
}

//Gameplay functions
bool do_dynamic_objects_motion_tick(struct repeating_timer *t) {
  uint8_t objects_speed = 2 + int((unsigned int)t->user_data / 4);
  std::vector<int> tmp_ids;
  for (int i = 0; i < spawned_objects.size(); ++i) {
    spawned_objects[i].current_position = {spawned_objects[i].current_position.x - objects_speed, spawned_objects[i].current_position.y};
    if (spawned_objects[i].current_position.x < 20) {
      tmp_ids.emplace_back(i);
    }
  }
  for (int i = 0; i < tmp_ids.size(); ++i) {
    auto element_to_delete = spawned_objects.begin() + tmp_ids[i];
    spawned_objects.erase(element_to_delete);
  }
  return true;
}

void render_dynamic_objects() {
  for (int i = 0; i < spawned_objects.size(); ++i) {
    spawned_objects[i].render(animation_counter);
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
  // Objects setup
  Object player_cat(CAT_OBJECT_PROTOTYPE);
  Object rainbow_tail(RAINBOW_OBJECT_PROTOTYPE);

  generateBackgroundStars();

  // --- DISPLAY, LED AND UI ELEMENTS SETUP ---
  initializeDisplay();

  // Animation ticks setup
  struct repeating_timer animation_timer;
  add_repeating_timer_ms(ANIMATION_TICK_PERIOD, animation_tick_function, NULL, &animation_timer);

  // Extra variables and timers
  struct repeating_timer rainbow_mode_timer;
  unsigned int score = 0;
  std::string score_text;
  std::string rainbow_mode_time_text;

  struct repeating_timer motion_timer;
  add_repeating_timer_ms(MOTION_TICK_PERIOD, do_dynamic_objects_motion_tick, (void*) score, &motion_timer);

  while (true) {
    // Display cleanup
    clearDisplay();

    //Background Stars animation change
    renderBackgroundStars(animation_counter);

    // Main program logic
    switch (game_state) {
      case GameState::title: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::game;
          player_cat.current_position = {20, 110};

          spawn_object_alarm = add_alarm_in_ms((MINIMAL_SPAWNER_INTERVAL + rand()) % MAXIMAL_SPAWNER_INTERVAL, object_spawner_function, NULL, false);
          break;
        }
        player_cat.current_position = {130, 140};
        rainbow_tail.current_position = {110, 140};

        // UI rendering
        disableLED();

        renderTitleText();

        rainbow_tail.render(animation_counter);
        player_cat.render(animation_counter);
        break;
      }
      case GameState::game: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::paused;

          if (spawn_object_alarm) {
            cancel_alarm(spawn_object_alarm);
            spawn_object_alarm = 0;
          }
          break;
        }
        if (buttonAPressed()) {
          player_cat.current_position = {player_cat.current_position.x,
                             (player_cat.current_position.y > 10) ? (player_cat.current_position.y - 2)
                                                                : player_cat.current_position.y};
        }
        if (buttonBPressed()) {
          player_cat.current_position = {player_cat.current_position.x,
                             (player_cat.current_position.y < 190) ? (player_cat.current_position.y + 2)
                                                                 : player_cat.current_position.y};
        }

        // --- GAME LOGIC ---

        // Rainbow mode handling
        if (rainbow_mode) {
          rainbow_tail.current_position = {player_cat.current_position.x - 20, player_cat.current_position.y};
          rainbow_tail.render(animation_counter);

          renderRainbowModeText(rainbow_time_value);

          setLEDColorHSV((float) pimoroni::millis() / 5000.0f, 1.0f, 0.5f + sinf(pimoroni::millis() / 100.0f / 3.14159f) * 0.5f);
        } else {
          disableLED();
        }

        player_cat.render(animation_counter);

        render_dynamic_objects();

        int collided_with = check_collisions_with_objects(player_cat);

        if (collided_with != INT32_MAX) {
          ObjectTypes collided_type = spawned_objects[collided_with].object_type;
          switch (collided_type) {
            case ObjectTypes::FISH: {
              score++;
              spawned_objects.erase(spawned_objects.begin() + collided_with);
              break;
            }
            case ObjectTypes::METEORITE: {
              if (rainbow_mode) {
                spawned_objects.erase(spawned_objects.begin() + collided_with);
              } else {
                spawned_objects.clear();
                game_state = GameState::game_over;
              }
              break;
            }
            case ObjectTypes::STAR: {
              if (rainbow_mode) cancel_repeating_timer(&rainbow_mode_timer);
              rainbow_mode = true;
              rainbow_time_value = RAINBOW_MODE_TIME_LENGTH;
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
      case GameState::paused: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::game;

          spawn_object_alarm = add_alarm_in_ms((MINIMAL_SPAWNER_INTERVAL + rand()) % MAXIMAL_SPAWNER_INTERVAL, object_spawner_function, NULL, false);
          break;
        }

        // UI rendering
        renderPausedGameText(score);

        break;
      }
      case GameState::game_over: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::title;
          break;
        }

        // Timers cleanup
        if (spawn_object_alarm) {
          cancel_alarm(spawn_object_alarm);
          spawn_object_alarm = 0;
        }

        if (rainbow_mode) {
          cancel_repeating_timer(&rainbow_mode_timer);
          rainbow_time_value = RAINBOW_MODE_TIME_LENGTH;
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