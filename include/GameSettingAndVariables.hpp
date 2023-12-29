#pragma once

// ----- GAMEPLAY SETTINGS -----

// Object spawner settings. Time in [ms]
const unsigned int MINIMAL_SPAWNER_INTERVAL {1500};
const unsigned int MAXIMAL_SPAWNER_INTERVAL {5000};

// Rainbow mode settings. Time in [s]
const short RAINBOW_MODE_TIME_LENGTH {20};

// Background stars settings
const short BACKGROUND_STARS_COUNT {30};
const short STARS_IN_ROW {6};

// ----- TIMERS SETTINGS -----
//For why these values are negative check Pico API (https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#repeating_timer)

// Period between animation changes ticks. Time in [ms].
const int ANIMATION_TICK_PERIOD {-100};

// Period between object position changes ticks. Time in [ms].
const int MOTION_TICK_PERIOD {-20};