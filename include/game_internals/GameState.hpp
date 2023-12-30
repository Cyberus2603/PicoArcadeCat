#pragma once

enum class GameState { IN_GAME, TITLE, PAUSED, GAME_OVER };
extern enum GameState game_state;

void handleTitleState();
void handleInGameState();
void handlePausedState();
void handleGameOverState();