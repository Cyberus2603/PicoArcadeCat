#pragma once

#include "ColorDefinitons.hpp"

#include <list>
#include <vector>
#include <utility>

struct ColoredRectangle {
  std::pair<int32_t, int32_t> start_point; //(0;0) is left upper corner
  int32_t width;
  int32_t height;
  PenColor color;
};

using Image = std::list<ColoredRectangle>;

using AnimatedImage = std::vector<Image>;