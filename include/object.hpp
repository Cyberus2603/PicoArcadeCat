#pragma once

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "ImageDefinitions.hpp"

enum ObjectTypes{ cat, background_star, fish, meteorite, rainbow_star, rainbow };

class Object {
 public:
  Object();
  Object(const VisualAsset* visual_asset,
         pimoroni::Rect collider,
         enum ObjectTypes object_type,
         int position_x = 0,
         int position_y = 0);
  void render(int pos_x, int pos_y, unsigned int animation_counter);
  int get_position_x();
  int get_position_y();
  pimoroni::Rect get_collider();
  ObjectTypes get_object_type();
  void set_pos(int pos_x, int pos_y);
  bool check_collision(Object& object_to_check);
 private:
  const VisualAsset* visual_asset;
  pimoroni::Rect collider;
  int pos_x;
  int pos_y;
  enum ObjectTypes obj_type;
};

