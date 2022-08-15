//
// Created by max on 28.06.22.
//

#include "image_frame.h"

Image_frame::Image_frame() = default;

Image_frame::Image_frame(PicoGraphics_PenRGB332 &graphics, uint8_t array[][7], int array_len) {
  parts = new Image_part[array_len];
  parts_size = array_len;
  for (int i = 0; i < array_len; ++i) {
    Image_part tmp;
    tmp.region = Rect(array[i][0], array[i][1], array[i][2], array[i][3]);
    tmp.pen_color = graphics.create_pen(array[i][4], array[i][5], array[i][6]);
    parts[i] = tmp;
  }
}

int Image_frame::GetPartsSize() const {
  return parts_size;
}

Image_part *Image_frame::GetParts() const {
  return parts;
}

