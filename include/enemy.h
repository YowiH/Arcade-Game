#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

class Enemy {
public:
    int speed;
    Vector2 velocity;
    Vector2 position;
    int hp;
};

#endif