#include <utility>

#include "game_object/GameObject.hpp"

GameObject::GameObject(const GameObjectType game_object_type, const GameObjectAssets* object_assets) : game_object_type(game_object_type), object_assets(object_assets) {}

void GameObject::setObjectPosition(std::pair<int32_t, int32_t> position) {
 object_position = position;
}

std::pair<int32_t, int32_t> GameObject::getObjectPosition() const {
  return object_position;
}

pimoroni::Rect GameObject::getObjectCollider() const {
  return object_assets->getCollider();
}

std::variant<Image, AnimatedImage> GameObject::getObjectVisualAsset() const {
  return object_assets->getVisualAsset();
}

GameObjectType GameObject::getGameObjectType() const {
  return game_object_type;
}



