#pragma once

#include "Object.hpp"

#include <vector>

extern std::vector<Object> spawned_objects;
void startObjectSpawnerTimer();
void stopObjectSpawnerTimer();
