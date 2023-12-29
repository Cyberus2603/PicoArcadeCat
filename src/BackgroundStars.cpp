#include "BackgroundStars.hpp"

#include "GameSettingAndVariables.hpp"
#include "object.hpp"
#include "Images.hpp"

Object background_stars[30];

void generateBackgroundStars() {
  for (int i = 0; i < BACKGROUND_STARS_COUNT; ++i) {
    background_stars[i] = Object(&BACKGROUND_STAR_VISUAL_ASSET, {0, 0, 0, 0}, ObjectTypes::BACKGROUND_STAR, 0, 0);
  }
}

void renderBackgroundStars(unsigned int animation_counter) {
  int sign_x = 1;
  int sign_y = 1;
  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < 5; ++j) {
      background_stars[4 * i + j].render(20 + (50 * i) + (10 * sign_x * sign_y),
                                         20 + (45 * j) + (5 * sign_y),
                                         animation_counter + (4 * i + j));
      sign_y *= -1;
    }
    sign_x *= -1;
  }
}