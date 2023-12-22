#include "game_object/GameObjectAssets.hpp"

GameObjectAssets::GameObjectAssets(const std::variant<Image, AnimatedImage> &visual_asset, const pimoroni::Rect &collider)
    : visual_asset(visual_asset), collider(collider) {}

const std::variant<Image, AnimatedImage> &GameObjectAssets::getVisualAsset() const {
  return visual_asset;
}

const pimoroni::Rect &GameObjectAssets::getCollider() const {
  return collider;
}
