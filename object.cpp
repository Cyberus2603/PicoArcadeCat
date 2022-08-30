//
// Created by max on 28.06.22.
//

#include "object.h"
Object::Object() = default;

Object::Object(Image_frame *frames,
               uint8_t frames_size,
               Rect *colliders,
               uint8_t colliders_size,
               int pos_x,
               int pos_y)
    : frames(frames),
      frames_size(frames_size),
      colliders(colliders),
      colliders_size(colliders_size),
      position_x(pos_x),
      position_y(pos_y) {}

void Object::render(PicoGraphics_PenRGB332 &graphics, int pos_x, int pos_y, uint8_t frame_number) {
  for (int j = 0; j < this->frames[frame_number].GetPartsSize(); ++j) {
    graphics.set_pen(this->frames[frame_number].GetParts()[j].pen_color);
    Rect tmp = this->frames[frame_number].GetParts()[j].region;
    tmp.x += pos_x;
    tmp.y += pos_y;
    graphics.rectangle(tmp);
  }
}

int Object::get_position_x() {
  return position_x;
}

int Object::get_position_y() {
  return position_y;
}

uint8_t Object::get_frames_size() {
  return frames_size;
}

Rect *Object::get_colliders() {
  return colliders;
}

uint8_t Object::get_colliders_size() {
  return colliders_size;
}

void Object::set_pos(int pos_x, int pos_y) {
  this->position_x = pos_x;
  this->position_y = pos_y;
}

bool Object::check_collision(Object object_to_check) {
  bool collision = false;
  for (int i = 0; i < this->colliders_size; ++i) {
    for (int j = 0; j < object_to_check.get_colliders_size(); ++j) {
      Rect tmp_collider_1 = Rect(this->position_x, this->position_y, this->colliders[i].w, this->colliders[i].h);
      Rect tmp_collider_2 = Rect(object_to_check.get_position_x(), object_to_check.get_position_y(),
                                 object_to_check.get_colliders()[j].w, object_to_check.get_colliders()[j].h);
      if (tmp_collider_1.intersects(tmp_collider_2)) collision = true;
    }
  }
  return collision;
}


