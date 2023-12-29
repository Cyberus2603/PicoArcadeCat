#include "GameTimers.hpp"

#include "assets/GameObjectsPrototypes.hpp"
#include "GameSettingAndVariables.hpp"
#include "Object.hpp"
#include "GameState.hpp"

#include <pico/time.h>

// Animation Timer
repeating_timer animation_timer;
unsigned int animation_counter {0};

bool animationTickFunction(struct repeating_timer *t) {
  ++animation_counter;
  return true;
}

void startAnimationTimer() {
  add_repeating_timer_ms(ANIMATION_TICK_PERIOD, animationTickFunction, NULL, &animation_timer);
}

unsigned int getAnimationTimerValue() {
  return animation_counter;
}

// Rainbow Timer
repeating_timer rainbow_mode_timer;

uint8_t rainbow_time_left {RAINBOW_MODE_TIME_LENGTH};
bool rainbow_mode {false};
bool rainbow_timer_started {false};

bool rainbowTimeTickFunction(struct repeating_timer *t) {
  if (game_state == GameState::PAUSED) {
    return true;
  }
  rainbow_time_left--;
  if (rainbow_time_left == 0) {
    rainbow_timer_started = false;
    rainbow_mode = false;
    return false;
  }
  return true;
}

bool RainbowModeIsEnabled() {
  return rainbow_mode;
}

unsigned int getRainbowModeTimeLeft() {
  return rainbow_time_left;
}

void startRainbowMode(){
  rainbow_mode = true;
  rainbow_time_left = RAINBOW_MODE_TIME_LENGTH;

  if (rainbow_timer_started) {
    return;
  }

  rainbow_timer_started = true;
  add_repeating_timer_ms(RAINBOW_MODE_TICK_PERIOD, rainbowTimeTickFunction, NULL, &rainbow_mode_timer);
}

void stopRainbowMode(){
  rainbow_timer_started = false;
  rainbow_mode = false;
  cancel_repeating_timer(&rainbow_mode_timer);
}


// Object Spawner
alarm_id_t spawn_object_alarm;
std::vector<Object> spawned_objects;

int64_t objectSpawnerFunction(alarm_id_t id, void *user_data) {
  int obj_type_to_spawn = rand() % (METEORITE_SPAWN_RATIO + FISH_SPAWN_RATIO + STAR_SPAWN_RATIO);

  if (obj_type_to_spawn < FISH_SPAWN_RATIO) {
    Object spawned_fish(FISH_OBJECT_PROTOTYPE);
    spawned_fish.current_position = {300, (20 + rand()) % 200};
    spawned_objects.emplace_back(spawned_fish);
  } else if (obj_type_to_spawn < (FISH_SPAWN_RATIO + METEORITE_SPAWN_RATIO)) {
    Object spawned_meteorite(METEORITE_OBJECT_PROTOTYPE);
    spawned_meteorite.current_position = {300, (20 + rand()) % 200};
    spawned_objects.emplace_back(spawned_meteorite);
  } else {
    Object spawned_star(STAR_OBJECT_PROTOTYPE);
    spawned_star.current_position = {300, (20 + rand()) % 200};
    spawned_objects.emplace_back(spawned_star);
  }
  cancel_alarm(spawn_object_alarm);
  spawn_object_alarm = add_alarm_in_ms((MINIMAL_SPAWNER_INTERVAL + rand()) % MAXIMAL_SPAWNER_INTERVAL,
                                       objectSpawnerFunction,
                                       NULL,
                                       false);
  return 0;
}

void startObjectSpawnerTimer(){
  spawn_object_alarm = add_alarm_in_ms((MINIMAL_SPAWNER_INTERVAL + rand()) % MAXIMAL_SPAWNER_INTERVAL, objectSpawnerFunction, NULL, false);
}

void stopObjectSpawnerTimer(){
  if (spawn_object_alarm) {
    cancel_alarm(spawn_object_alarm);
  }
}

// Objects Motion Timer
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