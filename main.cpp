#include "PimoroniDisplayHandler.hpp"

#include "assets/GameObjectsPrototypes.hpp"
#include "GameSettingAndVariables.hpp"
#include "GameTimers.hpp"
#include "GameStates.hpp"
#include "BackgroundStars.hpp"
#include "Object.hpp"
#include "UI.hpp"

//Gameplay functions
void render_dynamic_objects() {
  unsigned int animation_counter = getAnimationTimerValue();
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
  Object player_cat(CAT_OBJECT_PROTOTYPE);
  Object rainbow_tail(RAINBOW_OBJECT_PROTOTYPE);

  generateBackgroundStars();

  initializeDisplay();

  startAnimationTimer();

  unsigned int score {0};

  startObjectMotionTimer();

  while (true) {
    unsigned int animation_counter = getAnimationTimerValue();
    clearDisplay();
    renderBackgroundStars(animation_counter);

    // Main program logic
    switch (game_state) {
      case GameState::TITLE: {
        // Input handling
        if (buttonXClicked()) {
          score = 0;
          resetObjectsSpeed();
          game_state = GameState::IN_GAME;
          player_cat.current_position = {20, 110};

          startObjectSpawnerTimer();
          break;
        }
        player_cat.current_position = {130, 140};
        rainbow_tail.current_position = {110, 140};

        disableLED();

        renderTitleText();

        rainbow_tail.render(animation_counter);
        player_cat.render(animation_counter);
        break;
      }
      case GameState::IN_GAME: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::PAUSED;

          stopObjectSpawnerTimer();
          break;
        }
        if (buttonAPressed()) {
          player_cat.current_position = {player_cat.current_position.x,
                             (player_cat.current_position.y > 10) ? (player_cat.current_position.y - PLAYER_SPEED)
                                                                : player_cat.current_position.y};
        }
        if (buttonBPressed()) {
          player_cat.current_position = {player_cat.current_position.x,
                             (player_cat.current_position.y < 190) ? (player_cat.current_position.y + PLAYER_SPEED)
                                                                 : player_cat.current_position.y};
        }

        // --- GAME LOGIC ---

        // Rainbow mode handling
        if (RainbowModeIsEnabled()) {
          rainbow_tail.current_position = {player_cat.current_position.x - 20, player_cat.current_position.y};
          rainbow_tail.render(animation_counter);

          unsigned int rainbow_mode_time_left = getRainbowModeTimeLeft();
          renderRainbowModeText(rainbow_mode_time_left);

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
              ++score;
              increaseObjectsSpeed();
              spawned_objects.erase(spawned_objects.begin() + collided_with);
              break;
            }
            case ObjectTypes::METEORITE: {
              if (RainbowModeIsEnabled()) {
                spawned_objects.erase(spawned_objects.begin() + collided_with);
              } else {
                spawned_objects.clear();
                game_state = GameState::GAME_OVER;
              }
              break;
            }
            case ObjectTypes::STAR: {
              startRainbowMode();
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
      case GameState::PAUSED: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::IN_GAME;
          startObjectSpawnerTimer();
          break;
        }

        // UI rendering
        renderPausedGameText(score);

        break;
      }
      case GameState::GAME_OVER: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::TITLE;
          break;
        }

        stopObjectSpawnerTimer();
        stopRainbowMode();

        disableLED();
        renderGameOverText(score);
        break;
      }
    }
    updateDisplay();
  }
}