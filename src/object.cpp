#include "object.h"

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
    auto rectangle_color = COLORS.at(current_rectangle.color);
    graphics.set_pen(rectangle_color.red, rectangle_color.green, rectangle_color.blue);
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
  return collider.intersects(object_to_check.get_collider());
}



