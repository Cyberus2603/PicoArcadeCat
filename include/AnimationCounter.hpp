#pragma once

#include <pico/time.h>

class AnimationCounter {
 public:
  static AnimationCounter* getInstance();
  void startCounter();
  unsigned long long getCounterValue();
 private:
  AnimationCounter() = default;
  static bool animationTickFunction(struct repeating_timer *t);

  static inline AnimationCounter* instance {nullptr};

  const int ANIMATION_PERIOD {-100}; //Negative value to calculate callback time correctly, check pico API for details
  struct repeating_timer animation_timer {};
  bool is_started {false};
  static inline unsigned long long animation_counter {0};
};