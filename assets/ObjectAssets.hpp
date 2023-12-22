#pragma once

#include "Images.hpp"
#include "game_object/GameObjectAssets.hpp"

namespace object_assets {
  const static GameObjectAssets REWARD_FISH_ASSETS {
      visual_assets::FISH_IMAGE, {0, 0, 13 * visual_assets::PIXEL_SIZE, 8 * visual_assets::PIXEL_SIZE}
  };

  const static GameObjectAssets METEORITE_ASSETS {
      visual_assets::METEORITE_IMAGE, {0, 0, 23 * visual_assets::PIXEL_SIZE, 21 * visual_assets::PIXEL_SIZE}
  };

  const static GameObjectAssets STAR_ASSETS {
      visual_assets::STAR_IMAGE, {0, 0, 18 * visual_assets::PIXEL_SIZE, 18 * visual_assets::PIXEL_SIZE}
  };

  const static GameObjectAssets RAINBOW_ASSETS {
      visual_assets::RAINBOW_ANIMATED_IMAGE, {0, 0, 0, 0}
  };

  const static GameObjectAssets CAT_ASSETS {
      visual_assets::CAT_ANIMATED_IMAGE, {0, 0, 32 * visual_assets::PIXEL_SIZE, 18 * visual_assets::PIXEL_SIZE}
  };

  const static GameObjectAssets BACKGROUND_STAR_ASSETS {
      visual_assets::BACKGROUND_STAR_ANIMATED_IMAGE, {0, 0, 0, 0}
  };
}



