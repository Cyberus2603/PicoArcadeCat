#pragma once

#include <pico_graphics.hpp>
#include "pimoroni_display/ImageDefinitions.hpp"

enum class ObjectTypes{ CAT, BACKGROUND_STAR, FISH, METEORITE, STAR, RAINBOW };

struct Object {
  VisualAsset* visual_asset{};
  pimoroni::Rect collider;
  enum ObjectTypes object_type {};
  pimoroni::Point current_position;

  void render(unsigned int animation_counter);
  bool collidedWith(Object& object_to_check);
};
