#include "BackgroundStars.hpp"

#include "assets/GameObjectsPrototypes.hpp"
#include "GameSettingAndVariables.hpp"

#include "PimoroniDisplayHandler.hpp"
#include "Object.hpp"

Object background_stars[BACKGROUND_STARS_COUNT];

void generateBackgroundStars() {
  for (int i = 0; i < BACKGROUND_STARS_COUNT; ++i) {
    Object background_star(BACKGROUND_STAR_OBJECT_PROTOTYPE);
    background_stars[i] = background_star;
  }

  const int NUMBER_OF_ROWS = (BACKGROUND_STARS_COUNT % STARS_IN_ROW) ? ((BACKGROUND_STARS_COUNT / STARS_IN_ROW) + 1) : (BACKGROUND_STARS_COUNT / STARS_IN_ROW);
  const int DISTANCE_BETWEEN_STARS_X = SCREEN_WIDTH / (STARS_IN_ROW + 1);
  const int DISTANCE_BETWEEN_STARS_Y = SCREEN_HEIGHT / (NUMBER_OF_ROWS + 1);
  const int TILT_DISTANCE {10};
  int star_counter {0};
  for (int i = 0; i < STARS_IN_ROW; ++i) {
    for (int j = 0; j < NUMBER_OF_ROWS; ++j) {
      int position_x {DISTANCE_BETWEEN_STARS_X + (DISTANCE_BETWEEN_STARS_X * i) + ((star_counter % 2) * -TILT_DISTANCE)};
      int position_y {DISTANCE_BETWEEN_STARS_Y + (DISTANCE_BETWEEN_STARS_Y * j) - ((star_counter % 2) * -TILT_DISTANCE)};
      background_stars[star_counter].current_position = {position_x, position_y};
      ++star_counter;
    }
  }
}

void renderBackgroundStars(unsigned int animation_counter) {
  for (int i = 0; i < BACKGROUND_STARS_COUNT; ++i) {
    background_stars[i].render(animation_counter + i);
  }
}