#include "UIHandler.hpp"

namespace {
  const pimoroni::Point IN_GAME_SCORE_TEXT_LOCATION {130, 10};
  const pimoroni::Point RAINBOW_TIME_TEXT_LOCATION {90, 220};
  const pimoroni::Point TITLE_HEADER_TEXT_LOCATION {100, 80};
  const pimoroni::Point TITLE_PLAY_TEXT_LOCATION {85, 110};
  const pimoroni::Point PAUSED_HEADER_TEXT_LOCATION {120, 80};
  const pimoroni::Point PAUSED_SCORE_TEXT_LOCATION {130, 110};
  const pimoroni::Point PAUSED_RESUME_TEXT_LOCATION {80, 130};
  const pimoroni::Point GAME_OVER_HEADER_TEXT_LOCATION {90, 80};
  const pimoroni::Point GAME_OVER_SCORE_TEXT_LOCATION {130, 110};
  const pimoroni::Point GAME_OVER_RESTART_TEXT_LOCATION {80, 130};
}

UIHandler *UIHandler::getInstance() {
  if (instance == nullptr) {
    instance = new UIHandler();
    display_handler = DisplayHandler::getInstance();
  }
  return instance;
}

void UIHandler::renderScoreText(unsigned int score) {
  std::string score_text = "Score: " + std::to_string(score);
  display_handler->placeTextAtPosition(score_text, IN_GAME_SCORE_TEXT_LOCATION, font6);
}

void UIHandler::renderRainbowModeText(uint8_t time_left) {
  std::string rainbow_mode_time_text = "Rainbow left: " + std::to_string(time_left);
  display_handler->placeTextAtPosition(rainbow_mode_time_text, RAINBOW_TIME_TEXT_LOCATION, font6);
}

void UIHandler::renderTitleText() {
  display_handler->placeTextAtPosition("PICO CAT", TITLE_HEADER_TEXT_LOCATION, font14_outline);
  display_handler->placeTextAtPosition("Press X to start", TITLE_PLAY_TEXT_LOCATION);
}

void UIHandler::renderPausedGameText(unsigned int score) {
  std::string score_text = "Score: " + std::to_string(score);
  display_handler->placeTextAtPosition("PAUSED", PAUSED_HEADER_TEXT_LOCATION, font14_outline);
  display_handler->placeTextAtPosition(score_text, PAUSED_SCORE_TEXT_LOCATION);
  display_handler->placeTextAtPosition("Press X to unpause", PAUSED_RESUME_TEXT_LOCATION);
}

void UIHandler::renderGameOverText(unsigned int score) {
  std::string score_text = "Score: " + std::to_string(score);
  display_handler->placeTextAtPosition("GAME OVER", GAME_OVER_HEADER_TEXT_LOCATION, font14_outline);
  display_handler->placeTextAtPosition(score_text, GAME_OVER_SCORE_TEXT_LOCATION);
  display_handler->placeTextAtPosition("Press X to restart", GAME_OVER_RESTART_TEXT_LOCATION);
}
