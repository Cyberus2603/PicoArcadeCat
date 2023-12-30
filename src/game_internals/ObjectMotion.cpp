#include "game_internals/ObjectMotion.hpp"

#include "assets/GameObjectsPrototypes.hpp"
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

bool doObjectsMotionTick(struct repeating_timer *t) {
  std::vector<int> tmp_ids;
  for (int i = 0; i < spawned_objects.size(); ++i) {
    spawned_objects[i].current_position = {spawned_objects[i].current_position.x - (BASE_OBJECT_SPEED + objects_speed), spawned_objects[i].current_position.y};
    if (spawned_objects[i].current_position.x < 20) {
      tmp_ids.emplace_back(i);
    }
  }

  // TODO Move to separate function
  for (int i = 0; i < tmp_ids.size(); ++i) {
    auto element_to_delete = spawned_objects.begin() + tmp_ids[i];
    spawned_objects.erase(element_to_delete);
  }
  return true;
}

void startObjectMotionTimer() {
  add_repeating_timer_ms(MOTION_TICK_PERIOD, doObjectsMotionTick, NULL, &motion_timer);
}