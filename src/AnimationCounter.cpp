#include "AnimationCounter.hpp"

AnimationCounter *AnimationCounter::getInstance() {
  if (instance == nullptr) {
    instance = new AnimationCounter();
  }
  return instance;
}

bool AnimationCounter::animationTickFunction(struct repeating_timer *t) {
  ++animation_counter;
  return true;
}

void AnimationCounter::startCounter() {
  if (!is_started) {
    add_repeating_timer_ms(ANIMATION_PERIOD, animationTickFunction, nullptr, &animation_timer);
    is_started = true;
  }
}

unsigned long long AnimationCounter::getCounterValue() {
  return animation_counter;
}
