#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <vector>

class Enemy {
public:
    int speed;
    Vector2 velocity;
    Vector2 position;
    int hp;
};

#endif