#include "game_internals/AnimationTimer.hpp"

#include "game_internals/GameSettingAndVariables.hpp"

#include <pico/time.h>

repeating_timer animation_timer;
unsigned int animation_counter {0};

bool animationTickFunction(struct repeating_timer *t) {
  ++animation_counter;
  return true;
}

void startAnimationTimer() {
  add_repeating_timer_ms(ANIMATION_TICK_PERIOD, animationTickFunction, NULL, &animation_timer);
}

unsigned int getAnimationTimerValue() {
  return animation_counter;
}