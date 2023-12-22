#pragma once

#include "game_object/GameObject.hpp"
#include "game_object/GameObjectFactory.hpp"
#include "DisplayHandler.hpp"

class BackgroundStarsHandler {
 public:
  static BackgroundStarsHandler* getInstance();
  void generateStars();
  void renderStars(unsigned long long animation_counter);

 private:
  BackgroundStarsHandler() = default;

  static inline BackgroundStarsHandler* instance {nullptr};
  static inline GameObjectFactory* factory_instance {nullptr};
  static inline DisplayHandler* display_instance{nullptr};

  std::vector<GameObject> background_stars;
};