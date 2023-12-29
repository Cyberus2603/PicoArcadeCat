#include "Object.hpp"

#include "PimoroniDisplayHandler.hpp"

void Object::render(unsigned int animation_counter) {
  placeVisualAssetAtPosition(visual_asset, current_position, animation_counter);
}

bool Object::check_collision(Object& object_to_check) {
  pimoroni::Rect current_collider_with_position {current_position.x, current_position.y, collider.w, collider.h};
  pimoroni::Rect collider_to_check_with_position {object_to_check.current_position.x, object_to_check.current_position.y, object_to_check.collider.w, object_to_check.collider.h};
  return current_collider_with_position.intersects(collider_to_check_with_position);
}
