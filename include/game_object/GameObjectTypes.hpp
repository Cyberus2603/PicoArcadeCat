#pragma once

#include "ObjectAssets.hpp"

#include <map>

enum class GameObjectType {
  REWARD_FISH,
  METEORITE,
  STAR,
  RAINBOW,
  CAT,
  BACKGROUND_STAR
};

const static inline std::map<GameObjectType, GameObjectAssets> OBJECT_ASSETS {
    {GameObjectType::REWARD_FISH, object_assets::REWARD_FISH_ASSETS},
    {GameObjectType::METEORITE, object_assets::METEORITE_ASSETS},
    {GameObjectType::STAR, object_assets::STAR_ASSETS},
    {GameObjectType::RAINBOW, object_assets::RAINBOW_ASSETS},
    {GameObjectType::CAT, object_assets::CAT_ASSETS},
    {GameObjectType::BACKGROUND_STAR, object_assets::BACKGROUND_STAR_ASSETS}
};