#pragma once

#include "GameObjectTypes.hpp"
#include "GameObjectAssets.hpp"

#include <memory>

class GameObject {
 public:
  explicit GameObject(GameObjectType game_object_type, const GameObjectAssets* object_assets);
  void setObjectPosition(std::pair<int32_t, int32_t> position);
  [[nodiscard]] std::pair<int32_t, int32_t> getObjectPosition() const;
  [[nodiscard]] pimoroni::Rect getObjectCollider() const;
  [[nodiscard]] std::variant<Image, AnimatedImage> getObjectVisualAsset() const;
  [[nodiscard]] GameObjectType getGameObjectType() const;
 private:
  std::pair<int32_t, int32_t> object_position {0,0};
  const GameObjectAssets* object_assets {nullptr};
  const GameObjectType game_object_type;
};