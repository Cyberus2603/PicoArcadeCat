#include "Game.hpp"


namespace {
  constexpr std::pair<int32_t, int32_t> INITIAL_CAT_IN_GAME_POSITION {20, 110};
  constexpr std::pair<int32_t, int32_t> INITIAL_CAT_TITLE_POSITION {130, 140};
  constexpr std::pair<int32_t, int32_t> INITIAL_RAINBOW_TITLE_POSITION {110, 140};
  constexpr unsigned int MAXIMAL_POSITION_FROM_BOTTOM {190};
  constexpr unsigned int MAXIMAL_POSITION_FROM_TOP {10};
  constexpr unsigned int GAME_OBJECT_MOVEMENT_STEP {2};
  constexpr unsigned int GAME_OBJECT_MOVEMENT_SPEED_GAIN_DIVIDER {4};
  constexpr unsigned int RAINBOW_POSITION_OFFSET {20};
}

Game *Game::getInstance() {
  if (instance == nullptr) {
    instance = new Game();
    animation_counter = AnimationCounter::getInstance();
    game_object_factory = GameObjectFactory::getInstance();
    object_spawner = ObjectSpawner::getInstance();
    rainbow_mode_handler = RainbowModeHandler::getInstance();
    background_stars_handler = BackgroundStarsHandler::getInstance();
    ui_handler = UIHandler::getInstance();
    display_handler = DisplayHandler::getInstance();
  }
  return instance;
}

void Game::init() {
  display_handler->initialize();
  background_stars_handler->generateStars();
  player_cat = std::make_shared<GameObject>(game_object_factory->createObject(GameObjectType::CAT));
  player_rainbow = std::make_shared<GameObject>(game_object_factory->createObject(GameObjectType::RAINBOW));
  animation_counter->startCounter();
}

[[noreturn]] void Game::run() {
  while (true) {
    display_handler->beginRenderFrame();
    background_stars_handler->renderStars(animation_counter->getCounterValue());

    switch (game_state) {
      case GameState::GAME: {
        handleInGameState();
        break;
      }
      case GameState::TITLE:{
        handleTitleState();
        break;
      }
      case GameState::PAUSE:{
        handlePausedState();
        break;
      }
      case GameState::GAME_OVER:{
        handleGameOverState();
        break;
      }
    }
    display_handler->endRenderFrame();
  }
}

void Game::handleTitleState() {
  if (display_handler->isButtonXPressed()) {
    game_state = GameState::GAME;
    player_cat->setObjectPosition(INITIAL_CAT_IN_GAME_POSITION);
    object_spawner->clearSpawnedObjects();
    object_spawner->startSpawner();
  }
  player_cat->setObjectPosition(INITIAL_CAT_TITLE_POSITION);
  player_rainbow->setObjectPosition(INITIAL_RAINBOW_TITLE_POSITION);

  ui_handler->renderTitleText();

  display_handler->disableLED();
  renderGameObject(player_cat);
  renderGameObject(player_rainbow);
}

void Game::handleInGameState() {
  if (display_handler->isButtonXPressed()) {
    game_state = GameState::PAUSE;
    object_spawner->stopSpawner();
    rainbow_mode_handler->pauseCounter();
    return;
  }

  if (display_handler->isButtonAPressed()) {
    movePlayerUp();
  }

  if (display_handler->isButtonBPressed()) {
    movePlayerDown();
  }

  renderGameObject(player_cat);
  handleRainbowMode();
  handleSpawnedObjects();
  handleCollisions();

  ui_handler->renderScoreText(score);
}

void Game::handleRainbowMode() {
  if (rainbow_mode_handler->isRainbowModeEnabled()) {
    auto player_cat_position = player_cat->getObjectPosition();
    player_rainbow->setObjectPosition({player_cat_position.first - RAINBOW_POSITION_OFFSET, player_cat_position.second});
    renderGameObject(player_rainbow);

    ui_handler->renderRainbowModeText(rainbow_mode_handler->getRainbowModeTimeLeft());
    //TODO: Refactor
    display_handler->setLEDColorHSV((float) pimoroni::millis() / 5000.0f, 1.0f, 0.5f + sinf(pimoroni::millis() / 100.0f / 3.14159f) * 0.5f);
  } else {
    display_handler->disableLED();
  }
}

void Game::movePlayerDown() {
  auto current_player_position = player_cat->getObjectPosition();
  if (current_player_position.second < MAXIMAL_POSITION_FROM_BOTTOM) {
    player_cat->setObjectPosition({current_player_position.first, current_player_position.second + GAME_OBJECT_MOVEMENT_STEP});
  }
}

void Game::movePlayerUp() {
  auto current_player_position = player_cat->getObjectPosition();
  if (current_player_position.second > MAXIMAL_POSITION_FROM_TOP) {
    player_cat->setObjectPosition({current_player_position.first, current_player_position.second - GAME_OBJECT_MOVEMENT_STEP});
  }
}

void Game::handleSpawnedObjects() {
  object_spawner->removeOutOfScreenObjects();

  moveSpawnedObjects();
  renderSpawnedObjects();
}


void Game::renderSpawnedObjects() {
  for (const auto& game_object: object_spawner->getSpawnedGameObjects()) {
    renderGameObject(game_object);
  }
}

void Game::moveSpawnedObjects() {
  uint8_t objects_speed = GAME_OBJECT_MOVEMENT_STEP + (score / GAME_OBJECT_MOVEMENT_SPEED_GAIN_DIVIDER);

  for (auto& game_object: object_spawner->getSpawnedGameObjects()) {
    auto game_object_position = game_object.getObjectPosition();
    game_object.setObjectPosition({game_object_position.first - objects_speed, game_object_position.second});
  }
}

void Game::handleCollisions() {
  auto collision_status = checkIfCollided();

  if (collision_status.first) {
    auto collided_object = object_spawner->getSpawnedGameObjects().begin();
    std::advance(collided_object, collision_status.second);
    switch (collided_object->getGameObjectType()) {
      case GameObjectType::REWARD_FISH: {
        ++score;
        object_spawner->removeSpawnedObject(collision_status.second);
        break;
      }
      case GameObjectType::METEORITE: {
        if (rainbow_mode_handler->isRainbowModeEnabled()) {
          object_spawner->removeSpawnedObject(collision_status.second);
        } else {
          object_spawner->clearSpawnedObjects();
          game_state = GameState::GAME_OVER;
        }
        break;
      }
      case GameObjectType::STAR: {
        rainbow_mode_handler->startRainbowMode();
        object_spawner->removeSpawnedObject(collision_status.second);
        break;
      }
      default: {
        break;
      }
    }

  }
}

std::pair<bool, int> Game::checkIfCollided() {
  auto player_collider = player_cat->getObjectCollider();

  int collided_object_id {0};
  for (const auto& game_object: object_spawner->getSpawnedGameObjects()) {
    if (player_collider.intersects(game_object.getObjectCollider())) {
      return {true, collided_object_id};
    }
    ++collided_object_id;
  }
  return {false, 0};
}

void Game::handlePausedState() {
  if (display_handler->isButtonXPressed()) {
    game_state = GameState::GAME;
    object_spawner->startSpawner();
    rainbow_mode_handler->resumeCounter();
    return;
  }

  ui_handler->renderPausedGameText(score);
}

void Game::handleGameOverState() {
  if (display_handler->isButtonXPressed()) {
    game_state = GameState::TITLE;
    return;
  }

  object_spawner->stopSpawner();
  rainbow_mode_handler->endRainbowMode();
  display_handler->disableLED();
  ui_handler->renderGameOverText(score);
}

void Game::renderGameObject(std::shared_ptr<GameObject> game_object) {
  renderGameObject(*game_object);
}

void Game::renderGameObject(const GameObject &game_object) {
  auto visual_asset = game_object.getObjectVisualAsset();
  if (std::holds_alternative<AnimatedImage>(visual_asset)) {
    auto animated_image = std::get<AnimatedImage>(visual_asset);
    auto frame_number = animation_counter->getCounterValue() % animated_image.size();
    display_handler->placeAnimatedImageForRender(animated_image, game_object.getObjectPosition(), frame_number);
  } else {
    display_handler->placeImageForRender(std::get<Image>(visual_asset), game_object.getObjectPosition());
  }
}






