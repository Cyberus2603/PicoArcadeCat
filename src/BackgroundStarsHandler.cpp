#include "BackgroundStarsHandler.hpp"
BackgroundStarsHandler *BackgroundStarsHandler::getInstance() {
  if (instance == nullptr) {
    instance = new BackgroundStarsHandler();
    factory_instance = GameObjectFactory::getInstance();
    display_instance = DisplayHandler::getInstance();
  }
  return instance;
}

//TODO: Refactor
void BackgroundStarsHandler::generateStars() {
  const short STARS_AMOUNT {30};
  for (int star_id = 0; star_id < STARS_AMOUNT; ++star_id) {
    GameObject star = factory_instance->createObject(GameObjectType::BACKGROUND_STAR);
    int sign_x = 1;
    int sign_y = 1;
    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 5; ++j) {
        star.setObjectPosition({20 + (50 * i) + (10 * sign_x * sign_y),20 + (45 * j) + (5 * sign_y)});
        sign_y *= -1;
      }
      sign_x *= -1;
    }
    background_stars.emplace_back(std::move(star));
  }
}

void BackgroundStarsHandler::renderStars(unsigned long long int animation_counter) {
  for (auto star : background_stars) {
    auto visual_asset = std::get<AnimatedImage>(star.getObjectVisualAsset());
    display_instance->placeAnimatedImageForRender(visual_asset, star.getObjectPosition(), animation_counter % visual_asset.size());
  }
}
