#pragma once

#include "GameObject.hpp"
#include "GameObjectAssets.hpp"
#include "GameObjectTypes.hpp"

class GameObjectFactory {
 public:
  static GameObjectFactory* getInstance();
  GameObject createObject(GameObjectType object_type);

 private:
  GameObjectFactory() = default;

  static inline GameObjectFactory* instance {nullptr};
};