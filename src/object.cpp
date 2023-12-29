#include "object.hpp"

Object::Object() = default;

Object::Object(const VisualAsset* visual_asset,
               Rect collider,
               enum ObjectTypes object_type,
               int position_x,
               int position_y)
    : visual_asset(visual_asset),
      collider(collider),
      obj_type(object_type),
      pos_x(position_x),
      pos_y(position_y) {}

void Object::render(PicoGraphics_PenRGB332 &graphics, int pos_x, int pos_y, uint8_t  animation_counter) {
  uint8_t frame_number =  animation_counter % visual_asset->size();
  auto frame_to_render = visual_asset->at(frame_number);
  for (const auto& current_rectangle : frame_to_render) {
    graphics.set_pen(current_rectangle.color.red, current_rectangle.color.green, current_rectangle.color.blue);
    graphics.rectangle({current_rectangle.start_point.first + pos_x, current_rectangle.start_point.second + pos_y, current_rectangle.width, current_rectangle.height});
  }
}

int Object::get_position_x() {
  return pos_x;
}

int Object::get_position_y() {
  return pos_y;
}


Rect Object::get_collider() {
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



