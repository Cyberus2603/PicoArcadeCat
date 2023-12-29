#include "object.hpp"

#include "PimoroniDisplayHandler.hpp"

Object::Object() = default;

Object::Object(const VisualAsset* visual_asset,
               pimoroni::Rect collider,
               enum ObjectTypes object_type,
               int position_x,
               int position_y)
    : visual_asset(visual_asset),
      collider(collider),
      obj_type(object_type),
      pos_x(position_x),
      pos_y(position_y) {}

void Object::render(int pos_x, int pos_y, unsigned int animation_counter) {
  placeVisualAssetAtPosition(visual_asset, {pos_x, pos_y} , animation_counter);
}

int Object::get_position_x() {
  return pos_x;
}

int Object::get_position_y() {
  return pos_y;
}


pimoroni::Rect Object::get_collider() {
  return collider;
}


ObjectTypes Object::get_object_type() {
  return obj_type;
}

void Object::set_pos(int pos_x, int pos_y) {
  this->pos_x = pos_x;
  this->pos_y = pos_y;
}

bool Object::check_collision(Object& object_to_check) {
  pimoroni::Rect current_collider_with_position {pos_x, pos_y, collider.w, collider.h};
  pimoroni::Rect collider_to_check_with_position {object_to_check.pos_x, object_to_check.pos_y, object_to_check.collider.w, object_to_check.collider.h};
  return current_collider_with_position.intersects(collider_to_check_with_position);
}



