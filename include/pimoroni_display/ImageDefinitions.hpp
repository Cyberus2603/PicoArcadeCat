#pragma once

#include "pimoroni_display/ColorDefinitons.hpp"

#include <vector>
#include <utility>

struct ColoredRectangle {
  std::pair<int32_t, int32_t> start_point; //(0;0) is left upper corner
  int32_t width;
  int32_t height;
  ColorParts color;
};

using VisualAssetFrame = std::vector<ColoredRectangle>;

using VisualAsset = std::vector<VisualAssetFrame>;