#include "game_internals/ObjectSpawner.hpp"

#include "assets/GameObjectsPrototypes.hpp"
#include "game_internals/GameSettingAndVariables.hpp"
#include "game_internals/Object.hpp"

#include <pico/time.h>

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
