#include "game_internals/ObjectMotion.hpp"

#include "game_internals/GameSettingAndVariables.hpp"
#include "game_internals/ObjectSpawner.hpp"

#include <pico/time.h>

repeating_timer motion_timer;

int32_t objects_speed {0};

void resetObjectsSpeed() {
  objects_speed = 0;
}

void increaseObjectsSpeed() {
  if (objects_speed < MAXIMAL_OBJECT_SPEED) {
    ++objects_speed;
  }
}

std::vector<int> missed_objects_to_delete;

void deleteMissedObjects() {
  for (int i = 0; i < missed_objects_to_delete.size(); ++i) {
    auto element_to_delete = spawned_objects.begin() + missed_objects_to_delete[i];
    spawned_objects.erase(element_to_delete);
  }
}

bool doObjectsMotionTick(struct repeating_timer *t) {
  missed_objects_to_delete.clear();
  for (int i = 0; i < spawned_objects.size(); ++i) {
    spawned_objects[i].current_position = {spawned_objects[i].current_position.x - (BASE_OBJECT_SPEED + objects_speed), spawned_objects[i].current_position.y};
    if (spawned_objects[i].current_position.x < OBJECTS_POSITION_X_DELETE_THRESHOLD) {
      missed_objects_to_delete.emplace_back(i);
    }
  }

  deleteMissedObjects();
  return true;
}

void startObjectMotionTimer() {
  add_repeating_timer_ms(MOTION_TICK_PERIOD, doObjectsMotionTick, NULL, &motion_timer);
}

int checkCollisionsWithObjects(Object &player) {
  for (int i = 0; i < spawned_objects.size(); ++i) {
    if (player.collidedWith(spawned_objects[i])) {
      return i;
    }
  }
  return INT32_MAX;
}