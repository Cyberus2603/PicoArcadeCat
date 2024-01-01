#include "game_internals/RainbowMode.hpp"

#include "game_internals/GameSettingAndVariables.hpp"

#include <pico/time.h>

repeating_timer rainbow_mode_timer;

uint8_t rainbow_time_left {RAINBOW_MODE_TIME_LENGTH};
bool rainbow_mode {false};
bool rainbow_timer_started {false};
bool rainbow_mode_paused {false};

bool rainbowTimeTickFunction(struct repeating_timer *t) {
  if (rainbow_mode_paused) {
    return true;
  }
  rainbow_time_left--;
  if (rainbow_time_left == 0) {
    rainbow_timer_started = false;
    rainbow_mode = false;
    return false;
  }
  return true;
}

bool RainbowModeIsEnabled() {
  return rainbow_mode;
}

unsigned int getRainbowModeTimeLeft() {
  return rainbow_time_left;
}

void startRainbowMode(){
  rainbow_mode = true;
  rainbow_time_left = RAINBOW_MODE_TIME_LENGTH;

  if (rainbow_timer_started) {
    return;
  }

  rainbow_timer_started = true;
  add_repeating_timer_ms(RAINBOW_MODE_TICK_PERIOD, rainbowTimeTickFunction, NULL, &rainbow_mode_timer);
}

void stopRainbowMode(){
  rainbow_timer_started = false;
  rainbow_mode = false;
  cancel_repeating_timer(&rainbow_mode_timer);
}

void pauseRainbowMode() {
  rainbow_mode_paused = true;
}

void resumeRainbowMode() {
  rainbow_mode_paused = false;
}
