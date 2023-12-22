#pragma once

#include <cstdint>
#include <map>

struct ColorParts {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

enum class PenColor {
  WHITE,
  LIGHT_AQUA,
  AQUA,
  LIGHTER_DARK_AQUA,
  DARK_AQUA,
  LIGHT_GRAY,
  GRAY,
  DARK_GRAY,
  BROWN,
  CREAM,
  RED,
  ORANGE,
  YELLOW,
  LIME,
  GREEN,
  LIGHT_BLUE,
  BLUE,
  DARK_BLUE,
  DEEP_BLUE,
  PURPLE,
  LIGHT_PINK,
  PINK,
  DARK_PINK,
  BLACK,
};

const static std::map<PenColor, ColorParts> COLORS {
    { PenColor::WHITE, {255, 255, 255}},
    { PenColor::LIGHT_AQUA, {81, 254, 234}},
    { PenColor::AQUA, {25, 204, 185}},
    { PenColor::LIGHTER_DARK_AQUA, {23, 163, 154}},
    { PenColor::DARK_AQUA, {5, 125, 123}},
    { PenColor::LIGHT_GRAY, {170, 170, 170}},
    { PenColor::GRAY, {153, 153, 153}},
    { PenColor::DARK_GRAY, {48, 45, 37}},
    { PenColor::BROWN, {69, 51, 19}},
    { PenColor::CREAM, {201, 189, 155}},
    { PenColor::RED, {255, 0, 0}},
    { PenColor::ORANGE, {255, 131, 23}},
    { PenColor::YELLOW, {255, 255, 0}},
    { PenColor::LIME, {161, 255, 37}},
    { PenColor::GREEN, {51, 255, 0}},
    { PenColor::LIGHT_BLUE, {83, 215, 255}},
    { PenColor::BLUE, {0, 153, 255}},
    { PenColor::DARK_BLUE, {59, 132, 255}},
    { PenColor::DEEP_BLUE, {0, 50, 103}},
    { PenColor::PURPLE, {102, 51, 255}},
    { PenColor::LIGHT_PINK, {255, 153, 255}},
    { PenColor::PINK, {255, 111, 246}},
    { PenColor::DARK_PINK, {255, 51, 153}},
    { PenColor::BLACK, {0,0,0}}
};
