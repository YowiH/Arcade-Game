#pragma once
#include "walker.h"
#include "obstacle.h"
#include "bullet.h"

class Player : public Walker {
protected:
	int coins = 0;
	
	Vector2 player_mov_dir{ 0, 0 };
	int Mov_dir;

	int player_walk_anim_counter = 0;
	int player_death_anim = 0;
	
	bool player_right_foot;
	bool player_dying;

	bool xPosBlock = false, xNegBlock = false;
	bool yPosBlock = false, yNegBlock = false;

	std::vector<Bullet> bullet_tracker{};
	std::vector<Bullet> bullet_pool{};

	//shooting
	int fire_frame_counter = 0;
	int fire_rate = 15;
	Vector2 shoot_dir;
	int anim_dir; //variable to know what animation to play when shooting
public:
	Player();

	void move();
	void die();
	void be_attacked(std::vector<Enemy>& enemy_tracker);
	void be_restrained(float x, float y);
	void collide(std::vector<Obstacle>& obstacle_tracker);
	void collect();

	void shoot_bullets(Sound shoot_fx);
	void update_bullets();
	void bullet_attack();
	void bullet_collision();
};