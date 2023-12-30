#pragma once

#include "game_internals/Object.hpp"

void resetObjectsSpeed();
void increaseObjectsSpeed();
void startObjectMotionTimer();
int checkCollisionsWithObjects(Object &player);