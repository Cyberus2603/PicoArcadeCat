//
// Created by max on 28.06.22.
//

#ifndef NO_INTERNET_CAT_OBJECT_H
#define NO_INTERNET_CAT_OBJECT_H
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "image_frame.h"

using namespace pimoroni;

class Object {
 public:
  Object();
  Object(Image_frame *frames, uint8_t frames_size, Rect *colliders, uint8_t colliders_size);
  void render(PicoGraphics_PenRGB332 &graphics, int pos_x, int pos_y, uint8_t frame_number);
 protected:
  Image_frame *frames;
  uint8_t frames_size;
  Rect *colliders;
  uint8_t colliders_size;

};

#endif //NO_INTERNET_CAT_OBJECT_H
