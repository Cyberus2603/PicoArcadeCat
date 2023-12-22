#pragma once

#include "GameState.hpp"
#include "AnimationCounter.hpp"
#include "game_object/GameObjectFactory.hpp"
#include "ObjectSpawner.hpp"
#include "BackgroundStarsHandler.hpp"
#include "RainbowModeHandler.hpp"
#include "UIHandler.hpp"

class Game {
 public:
  static Game* getInstance();
  void init();
  [[noreturn]] void run();
 private:
  Game() = default;
  void handleTitleState();
  void handleInGameState();
  void movePlayerUp();
  void movePlayerDown();
  void handleRainbowMode();
  void handleSpawnedObjects();
  void renderSpawnedObjects();
  void moveSpawnedObjects();
  void handleCollisions();
  std::pair<bool, int> checkIfCollided();
  void handlePausedState();
  void handleGameOverState();
  void renderGameObject(std::shared_ptr<GameObject> game_object);
  void renderGameObject(const GameObject &game_object);

  static inline Game* instance {nullptr};
  static inline AnimationCounter* animation_counter {nullptr};
  static inline GameObjectFactory* game_object_factory {nullptr};
  static inline ObjectSpawner* object_spawner {nullptr};
  static inline RainbowModeHandler* rainbow_mode_handler {nullptr};
  static inline BackgroundStarsHandler* background_stars_handler {nullptr};
  static inline UIHandler* ui_handler {nullptr};
  static inline DisplayHandler* display_handler {nullptr};

  GameState game_state {GameState::TITLE};
  unsigned int score {0};

  std::shared_ptr<GameObject> player_cat {nullptr};
  std::shared_ptr<GameObject> player_rainbow {nullptr};
};