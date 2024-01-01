#include "pimoroni_display/PimoroniDisplayHandler.hpp"

#include "assets/GameObjectsPrototypes.hpp"
#include "game_internals/GameSettingAndVariables.hpp"
#include "game_internals/AnimationTimer.hpp"
#include "game_internals/RainbowMode.hpp"
#include "game_internals/ObjectMotion.hpp"
#include "game_internals/ObjectSpawner.hpp"
#include "game_internals/BackgroundStars.hpp"
#include "game_internals/Object.hpp"
#include "game_internals/UI.hpp"

enum class GameState { IN_GAME, TITLE, PAUSED, GAME_OVER };

int main() {
  Object player_cat(CAT_OBJECT_PROTOTYPE);
  Object rainbow_tail(RAINBOW_OBJECT_PROTOTYPE);
  GameState game_state {GameState::TITLE};

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
          player_cat.current_position = {INITIAL_IN_GAME_CAT_POSITION_X, INITIAL_IN_GAME_CAT_POSITION_Y};

          startObjectSpawnerTimer();
          break;
        }

        // Title screen objects positioning and rendering
        player_cat.current_position = {TITLE_SCREEN_CAT_POSITION_X, TITLE_SCREEN_CAT_POSITION_Y};
        rainbow_tail.current_position = {TITLE_SCREEN_RAINBOW_POSITION_X, TITLE_SCREEN_RAINBOW_POSITION_Y};
        rainbow_tail.render(animation_counter);
        player_cat.render(animation_counter);

        // UI and LED handling
        disableLED();
        renderTitleText();

        break;
      }
      case GameState::IN_GAME: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::PAUSED;

          pauseRainbowMode();
          stopObjectSpawnerTimer();
          break;
        }
        if (buttonAPressed()) {
          player_cat.current_position = {player_cat.current_position.x,
                             (player_cat.current_position.y > MINIMAL_CAT_POSITION_Y) ? (player_cat.current_position.y - PLAYER_SPEED)
                                                                : player_cat.current_position.y};
        }
        if (buttonBPressed()) {
          player_cat.current_position = {player_cat.current_position.x,
                             (player_cat.current_position.y < MAXIMAL_CAT_POSITION_Y) ? (player_cat.current_position.y + PLAYER_SPEED)
                                                                 : player_cat.current_position.y};
        }

        // --- GAME LOGIC ---

        // Objects rendering
        if (RainbowModeIsEnabled()) {
          rainbow_tail.current_position =
              {player_cat.current_position.x - RAINBOW_OFFSET, player_cat.current_position.y};
          rainbow_tail.render(animation_counter);
        }

        player_cat.render(animation_counter);
        renderSpawnedObjects();

        // Collision handling
        int collided_with = checkCollisionsWithObjects(player_cat);
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

        // UI handling
        renderScoreText(score);

        if (RainbowModeIsEnabled()) {
          unsigned int rainbow_mode_time_left = getRainbowModeTimeLeft();
          renderRainbowModeText(rainbow_mode_time_left);

          // Equation below is responsible for rainbow like color change on diode
          setLEDColorHSV((float) pimoroni::millis() / 5000.0f, 1.0f, 0.5f + sinf(pimoroni::millis() / 100.0f / 3.14159f) * 0.5f);
        } else {
          disableLED();
        }

        break;
      }
      case GameState::PAUSED: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::IN_GAME;
          resumeRainbowMode();
          startObjectSpawnerTimer();
          break;
        }

        // UI handling
        renderPausedGameText(score);

        break;
      }
      case GameState::GAME_OVER: {
        // Input handling
        if (buttonXClicked()) {
          game_state = GameState::TITLE;
          break;
        }

        // Condition based game systems reset
        stopObjectSpawnerTimer();
        stopRainbowMode();

        // UI and LED handling
        disableLED();
        renderGameOverText(score);

        break;
      }
    }
    updateDisplay();
  }
}