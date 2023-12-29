#pragma once

#include "Object.hpp"

#include <vector>

// Animation Timer
void startAnimationTimer();
unsigned int getAnimationTimerValue();

// RainbowTimer
bool RainbowModeIsEnabled();
unsigned int getRainbowModeTimeLeft();
void startRainbowMode();
void stopRainbowMode();

// Object Spawner
extern std::vector<Object> spawned_objects;
void startObjectSpawnerTimer();
void stopObjectSpawnerTimer();

// Objects Motion Timer
void resetObjectsSpeed();
void increaseObjectsSpeed();
void startObjectMotionTimer();