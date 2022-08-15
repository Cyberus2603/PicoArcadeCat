//
// Created by max on 28.06.22.
//

#include "object.h"
Object::Object() = default;

Object::Object(Image_frame *frames, uint8_t frames_size, Rect *colliders, uint8_t colliders_size)
    : frames(frames), frames_size(frames_size), colliders(colliders), colliders_size(colliders_size) {}


void Object::render(PicoGraphics_PenRGB332 &graphics, int pos_x, int pos_y, uint8_t frame_number) {
    for (int j = 0; j < this->frames[frame_number].GetPartsSize(); ++j) {
      graphics.set_pen(this->frames[frame_number].GetParts()[j].pen_color);
      Rect tmp = this->frames[frame_number].GetParts()[j].region;
      tmp.x += pos_x;
      tmp.y += pos_y;
      graphics.rectangle(tmp);
    }
}
