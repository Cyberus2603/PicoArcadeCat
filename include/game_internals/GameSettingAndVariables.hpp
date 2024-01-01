#pragma once

#include <cstdint>

// ----- GAMEPLAY SETTINGS -----

// Object spawner settings. Time in [ms]
const unsigned int MINIMAL_SPAWNER_INTERVAL {1500};
const unsigned int MAXIMAL_SPAWNER_INTERVAL {4000};

// Rainbow mode settings. Time in [s]
const short RAINBOW_MODE_TIME_LENGTH {10};

// Background stars settings
const short BACKGROUND_STARS_COUNT {30};
const short STARS_IN_ROW {6};

// Object speed settings. Unit is pixels on screen per motion timer tick
const int32_t PLAYER_SPEED {4};
const int32_t BASE_OBJECT_SPEED {2};
const int32_t MAXIMAL_OBJECT_SPEED {6};

// Object spawner setting. The bigger the number the more often this object is spawned
const unsigned int METEORITE_SPAWN_RATIO {60};
const unsigned int FISH_SPAWN_RATIO {30};
const unsigned int STAR_SPAWN_RATIO {10};

// ----- INITIAL POSITIONS AND LIMITS-----
// Gameplay limits for player movement
const unsigned int MINIMAL_CAT_POSITION_Y {10};
const unsigned int MAXIMAL_CAT_POSITION_Y {190}; // Not as close to screen height due to cat size

// Initial Positions
const unsigned int INITIAL_IN_GAME_CAT_POSITION_X {20};
const unsigned int INITIAL_IN_GAME_CAT_POSITION_Y {110};
const unsigned int TITLE_SCREEN_CAT_POSITION_X {130};
const unsigned int TITLE_SCREEN_CAT_POSITION_Y {140};
const unsigned int TITLE_SCREEN_RAINBOW_POSITION_X {110};
const unsigned int TITLE_SCREEN_RAINBOW_POSITION_Y {140};

// ----- Auxiliary constants -----
// Rainbow object offset from cat position
const int32_t RAINBOW_OFFSET {20};

// When to delete objects from screen when player avoids or misses them
const int32_t OBJECTS_POSITION_X_DELETE_THRESHOLD {20};

// ----- TIMERS SETTINGS -----
//For why these values are negative check Pico API (https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#repeating_timer)

// Period between animation changes ticks. Time in [ms].
const int ANIMATION_TICK_PERIOD {-100};

// Period between object position changes ticks. Time in [ms].
const int MOTION_TICK_PERIOD {-40};

// Period for rainbow mode one second ticks. Time in [ms].
const int RAINBOW_MODE_TICK_PERIOD {-1000};