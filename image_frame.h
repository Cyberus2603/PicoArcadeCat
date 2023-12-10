//
// Created by max on 28.06.22.
//

#ifndef NO_INTERNET_CAT_IMAGE_FRAME_H
#define NO_INTERNET_CAT_IMAGE_FRAME_H
#include "libraries/pico_graphics/pico_graphics.hpp"

using namespace pimoroni;

struct Image_part {
  Rect region;
  Pen pen_color;

  Image_part() {
    region = Rect();
    pen_color = Pen();
  }
};

class Image_frame {
 public:
  Image_frame();
  Image_frame(PicoGraphics_PenRGB332 &graphics, const volatile uint8_t array[][7], int array_len);
  int GetPartsSize() const;
  Image_part *GetParts() const;
 protected:
  Image_part *parts;
  int parts_size;
};

#endif //NO_INTERNET_CAT_IMAGE_FRAME_H
