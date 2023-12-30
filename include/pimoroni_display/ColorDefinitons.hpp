#pragma once

#include <cstdint>

struct ColorParts {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

namespace Colors {
    const inline ColorParts WHITE {255, 255, 255};
    const inline ColorParts LIGHT_AQUA {81, 254, 234};
    const inline ColorParts AQUA {25, 204, 185};
    const inline ColorParts LIGHTER_DARK_AQUA {23, 163, 154};
    const inline ColorParts DARK_AQUA {5, 125, 123};
    const inline ColorParts LIGHT_GRAY {170, 170, 170};
    const inline ColorParts GRAY {153, 153, 153};
    const inline ColorParts DARK_GRAY {48, 45, 37};
    const inline ColorParts BROWN {69, 51, 19};
    const inline ColorParts CREAM {201, 189, 155};
    const inline ColorParts RED {255, 0, 0};
    const inline ColorParts ORANGE {255, 131, 23};
    const inline ColorParts YELLOW {255, 255, 0};
    const inline ColorParts LIME {161, 255, 37};
    const inline ColorParts GREEN {51, 255, 0};
    const inline ColorParts LIGHT_BLUE {83, 215, 255};
    const inline ColorParts BLUE {0, 153, 255};
    const inline ColorParts DARK_BLUE {59, 132, 255};
    const inline ColorParts DEEP_BLUE {0, 50, 103};
    const inline ColorParts PURPLE {102, 51, 255};
    const inline ColorParts LIGHT_PINK {255, 153, 255};
    const inline ColorParts PINK {255, 111, 246};
    const inline ColorParts DARK_PINK {255, 51, 153};
    const inline ColorParts BLACK {0,0,0};
};
