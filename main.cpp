#include "Game.hpp"

int main() {
  Game* game = Game::getInstance();

  game->init();
  game->run();
}