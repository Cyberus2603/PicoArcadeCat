#pragma once

#include "game_object/GameObject.hpp"
#include "game_object/GameObjectFactory.hpp"

#include <pico/time.h>

#include <list>

class ObjectSpawner {
 public:
  static ObjectSpawner* getInstance();
  void startSpawner();
  void stopSpawner();
  void removeOutOfScreenObjects();
  void removeSpawnedObject(int object_id);
  void clearSpawnedObjects();
  std::list<GameObject>& getSpawnedGameObjects();
 private:
  ObjectSpawner() = default;
  static int64_t object_spawner_function(alarm_id_t id, void *user_data);

  const static inline int REWARD_FISH_CHANCE {30};
  const static inline int METEORITE_CHANCE {60};
  const static inline int STAR_CHANCE {10};
  static inline alarm_id_t spawn_object_alarm {};

  static inline ObjectSpawner* instance {nullptr};
  static inline GameObjectFactory* object_factory {nullptr};
  const static inline unsigned int MINIMAL_SPAWN_TIME {1500};
  const static inline unsigned int MAXIMAL_SPAWN_TIME {5000};

  static inline std::list<GameObject> spawned_objects;
};

