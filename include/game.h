#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "resource_dir.h"

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "spawn.h"
#include "obstacle.h"
#include "area.h"

class Game {
public:
	int fire_rate;
	int fire_rate_counter;
	

	Player player;
	Obstacle obstacle;
	Area desert;
	Bullet defaultBullet;

	void InitGame();
	void UpdateGame();
	void DrawGame();
	void UpdateDrawFrame();
	void UnloadGame();
};

#endif

