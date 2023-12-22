#pragma once

#include "ImageDefinitions.hpp"

#include <libraries/pico_graphics/pico_graphics.hpp>

#include <variant>

class GameObjectAssets {
 public:
  GameObjectAssets() = delete;
  GameObjectAssets(const std::variant<Image, AnimatedImage> &visual_asset, const pimoroni::Rect &collider);
  [[nodiscard]] const std::variant<Image, AnimatedImage> &getVisualAsset() const;
  [[nodiscard]] const pimoroni::Rect &getCollider() const;
 private:
  std::variant<Image, AnimatedImage> visual_asset;
  pimoroni::Rect collider;
};