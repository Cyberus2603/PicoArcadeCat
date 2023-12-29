#pragma once

#include <pico_graphics.hpp>

#include "ImageDefinitions.hpp"

void initializeDisplay();
void clearDisplay();
void updateDisplay();
bool buttonXPressed();
bool buttonXClicked();
bool buttonYPressed();
bool buttonYClicked();
bool buttonAPressed();
bool buttonAClicked();
bool buttonBPressed();
bool buttonBClicked();
void disableLED();
void setLEDColorHSV(float hue, float saturation, float value);
void placeTextAtPosition(const std::string& text, const pimoroni::Point &position, const bitmap::font_t& font = font8, const ColorParts& text_color = Colors::WHITE);
void placeVisualAssetAtPosition(const VisualAsset* visual_asset, const pimoroni::Point &position, unsigned int frame_number);