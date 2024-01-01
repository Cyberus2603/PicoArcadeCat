#pragma once

#include <cstdint>

void renderScoreText(unsigned int score);
void renderRainbowModeText(uint8_t time_left);
void renderTitleText();
void renderPausedGameText(unsigned int score);
void renderGameOverText(unsigned int score);