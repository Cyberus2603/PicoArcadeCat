#pragma once

#include "DisplayHandler.hpp"

class UIHandler {
 public:
  static UIHandler* getInstance();
  void renderScoreText(unsigned int score);
  void renderRainbowModeText(uint8_t time_left);
  void renderTitleText();
  void renderPausedGameText(unsigned int score);
  void renderGameOverText(unsigned int score);
 private:
  UIHandler() = default;

  static inline UIHandler* instance {nullptr};
  static inline DisplayHandler* display_handler {nullptr};
};
