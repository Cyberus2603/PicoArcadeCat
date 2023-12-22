#include "game_object/GameObjectFactory.hpp"

GameObjectFactory *GameObjectFactory::getInstance() {
  if (instance == nullptr) {
    instance = new GameObjectFactory();
  }
  return instance;
}

GameObject GameObjectFactory::createObject(GameObjectType object_type) {
  return GameObject(object_type,&OBJECT_ASSETS.at(object_type));
}
