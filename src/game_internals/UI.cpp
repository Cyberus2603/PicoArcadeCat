#include "game_internals/UI.hpp"

#include "pimoroni_display/PimoroniDisplayHandler.hpp"

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

void renderScoreText(unsigned int score) {
  std::string score_text = "Score: " + std::to_string(score);
  placeTextAtPosition(score_text, IN_GAME_SCORE_TEXT_LOCATION, font6);
}

void renderRainbowModeText(uint8_t time_left) {
  std::string rainbow_mode_time_text = "Rainbow left: " + std::to_string(time_left);
  placeTextAtPosition(rainbow_mode_time_text, RAINBOW_TIME_TEXT_LOCATION, font6);
}

void renderTitleText() {
  placeTextAtPosition("PICO CAT", TITLE_HEADER_TEXT_LOCATION, font14_outline);
  placeTextAtPosition("Press X to start", TITLE_PLAY_TEXT_LOCATION);
}

void renderPausedGameText(unsigned int score) {
  std::string score_text = "Score: " + std::to_string(score);
  placeTextAtPosition("PAUSED", PAUSED_HEADER_TEXT_LOCATION, font14_outline);
  placeTextAtPosition(score_text, PAUSED_SCORE_TEXT_LOCATION);
  placeTextAtPosition("Press X to unpause", PAUSED_RESUME_TEXT_LOCATION);
}

void renderGameOverText(unsigned int score) {
  std::string score_text = "Score: " + std::to_string(score);
  placeTextAtPosition("GAME OVER", GAME_OVER_HEADER_TEXT_LOCATION, font14_outline);
  placeTextAtPosition(score_text, GAME_OVER_SCORE_TEXT_LOCATION);
  placeTextAtPosition("Press X to restart", GAME_OVER_RESTART_TEXT_LOCATION);
}