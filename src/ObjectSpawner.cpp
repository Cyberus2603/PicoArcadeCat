#include "ObjectSpawner.hpp"

namespace {
  constexpr unsigned int OUT_OF_MAP_REMOVAL_THRESHOLD {20};
}

ObjectSpawner *ObjectSpawner::getInstance() {
  if (instance == nullptr) {
    instance = new ObjectSpawner();
    object_factory = GameObjectFactory::getInstance();
  }
  return instance;
}

void ObjectSpawner::startSpawner() {
  spawn_object_alarm = add_alarm_in_ms((MINIMAL_SPAWN_TIME + rand()) % MAXIMAL_SPAWN_TIME, object_spawner_function,
                                       nullptr, false);
}

void ObjectSpawner::stopSpawner() {
  cancel_alarm(spawn_object_alarm);
}

void ObjectSpawner::removeOutOfScreenObjects() {
  auto iterator = spawned_objects.begin();
  while (iterator != spawned_objects.end()) {
    auto object_position = iterator->getObjectPosition();
    if (object_position.first < OUT_OF_MAP_REMOVAL_THRESHOLD) {
      spawned_objects.erase(iterator++);
    }
  }
}

void ObjectSpawner::removeSpawnedObject(int object_id) {
  auto iterator = spawned_objects.begin();
  std::advance(iterator, object_id);
  spawned_objects.erase(iterator);
}

void ObjectSpawner::clearSpawnedObjects() {
  spawned_objects.clear();
}

std::list<GameObject> &ObjectSpawner::getSpawnedGameObjects() {
  return spawned_objects;
}

//TODO: Refactor and cleanup
int64_t ObjectSpawner::object_spawner_function(alarm_id_t id, void *user_data) {
    int obj_type_to_spawn = rand() % (REWARD_FISH_CHANCE + METEORITE_CHANCE + STAR_CHANCE);

    if (obj_type_to_spawn < REWARD_FISH_CHANCE) {
      GameObject tmp = object_factory->createObject(GameObjectType::REWARD_FISH);
      tmp.setObjectPosition({300, (20 + rand()) % 200});
      spawned_objects.emplace_back(std::move(tmp));
    } else if (obj_type_to_spawn < (REWARD_FISH_CHANCE + METEORITE_CHANCE)) {
      GameObject tmp = object_factory->createObject(GameObjectType::METEORITE);
      tmp.setObjectPosition({300, (20 + rand()) % 200});
      spawned_objects.emplace_back(std::move(tmp));
    } else {
      GameObject tmp = object_factory->createObject(GameObjectType::STAR);
      tmp.setObjectPosition({300, (20 + rand()) % 200});
      spawned_objects.emplace_back(std::move(tmp));
    }
    cancel_alarm(spawn_object_alarm);
    spawn_object_alarm = add_alarm_in_ms((MINIMAL_SPAWN_TIME + rand()) % MAXIMAL_SPAWN_TIME, object_spawner_function,
                                         nullptr, false);
    return 0;
}



