#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "spawn.h"

void InitGame();
void UpdateGame();
void DrawGame();
void UpdateDrawFrame();
void UnloadGame();

#endif

