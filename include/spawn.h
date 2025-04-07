#ifndef SPAWN_H
#define SPAWN_H

#include <vector>

#include "bullet.h"
#include "enemy.h"

std::vector<Bullet> bullet_tracker;
std::vector<Bullet> bullet_pool;

std::vector<Enemy> enemy_tracker;
std::vector<Enemy> enemy_pool;

#endif