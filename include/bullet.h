#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include <vector>

class Bullet {
public:
    int speed;
    Vector2 velocity;
    Vector2 position;
    int damage;
};

#endif