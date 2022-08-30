//
// Created by max on 28.06.22.
//

#ifndef NO_INTERNET_CAT_OBJECT_H
#define NO_INTERNET_CAT_OBJECT_H
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "image_frame.h"

using namespace pimoroni;

enum ObjectTypes{ cat, background_star, fish, meteorite, rainbow_star, rainbow };

class Object {
 public:
  Object();
  Object(Image_frame *frames,
         uint8_t frames_size,
         Rect *colliders,
         uint8_t colliders_size,
         enum ObjectTypes object_type,
         int position_x = 0,
         int position_y = 0);
  void render(PicoGraphics_PenRGB332 &graphics, int pos_x, int pos_y, uint8_t frame_number);
  int get_position_x();
  int get_position_y();
  uint8_t get_frames_size();
  Rect* get_colliders();
  uint8_t get_colliders_size();
  ObjectTypes get_object_type();
  void set_pos(int pos_x, int pos_y);
  bool check_collision(Object object_to_check);
 protected:
  Image_frame *frames;
  uint8_t frames_size;
  Rect *colliders;
  uint8_t colliders_size;
  int pos_x;
  int pos_y;
  enum ObjectTypes obj_type;
};

#endif //NO_INTERNET_CAT_OBJECT_H
