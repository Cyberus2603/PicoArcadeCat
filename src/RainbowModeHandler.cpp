#include "RainbowModeHandler.hpp"

RainbowModeHandler *RainbowModeHandler::getInstance() {
  if (instance == nullptr) {
    instance = new RainbowModeHandler();
  }
  return instance;
}

void RainbowModeHandler::startRainbowMode() {
  rainbow_time_value = RAINBOW_TIME_LENGTH;
  rainbow_mode = true;
  startCounter();
}

void RainbowModeHandler::endRainbowMode() {
  stopCounter();
  rainbow_mode = false;
}

void RainbowModeHandler::resumeCounter() {
  is_paused = false;
}

void RainbowModeHandler::pauseCounter() {
  is_paused = true;
}

void RainbowModeHandler::startCounter() {
  if (!is_started) {
    add_repeating_timer_ms(RAINBOW_MODE_TICK_PERIOD, rainbowTimeTickFunction, nullptr, &rainbow_mode_timer);
    is_started = true;
    is_started = false;
  }
}

void RainbowModeHandler::stopCounter() {
  if (is_started) {
    cancel_repeating_timer(&rainbow_mode_timer);
    is_paused = false;
    is_started = false;
  }
}

bool RainbowModeHandler::isRainbowModeEnabled() {
  return rainbow_mode;
}

uint8_t RainbowModeHandler::getRainbowModeTimeLeft() {
  return rainbow_time_value;
}

bool RainbowModeHandler::rainbowTimeTickFunction(struct repeating_timer *t) {
  if (is_paused)  {
    return true;
  }

  if (rainbow_time_value > 0) {
    rainbow_time_value--;
  }

  if (rainbow_time_value == 0) {
    rainbow_mode = false;
  }
  return true;
}







