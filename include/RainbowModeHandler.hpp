#pragma once

#include <pico/time.h>
#include <cstdint>

class RainbowModeHandler {
 public:
  static RainbowModeHandler* getInstance();
  void startRainbowMode();
  void endRainbowMode();
  void resumeCounter();
  void pauseCounter();
  bool isRainbowModeEnabled();
  uint8_t getRainbowModeTimeLeft();

 private:
  RainbowModeHandler() = default;
  void startCounter();
  void stopCounter();

  static bool rainbowTimeTickFunction(struct repeating_timer *t);

  static inline RainbowModeHandler* instance {nullptr};

  const int RAINBOW_MODE_TICK_PERIOD {-1000}; //Negative value to calculate callback time correctly, check pico API for details
  struct repeating_timer rainbow_mode_timer {};

  const static inline uint8_t RAINBOW_TIME_LENGTH {20};
  static inline uint8_t rainbow_time_value {RAINBOW_TIME_LENGTH};
  static inline bool rainbow_mode {false};
  static inline bool is_started {false};
  static inline bool is_paused {false};
};