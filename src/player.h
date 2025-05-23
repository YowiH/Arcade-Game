#pragma once
#include "walker.h"
#include "obstacle.h"
#include "bullet.h"
#include "powerUp.h"

class Player : public Walker {
protected:
	int coins = 0;
	
	Vector2 player_mov_dir{ 0, 0 };
	int mov_dir;

	int walk_anim_counter = 0;
	int death_anim = 0;
	
	bool right_foot;
	bool is_dying;

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
	bool get_is_dying() const;
	int get_coins() const;
	int get_mov_dir() const;
	int get_walk_anim_counter() const;
	int get_right_foot() const;
	std::vector<Bullet> get_bullet_tracker() const;

	void add_coins(int amount);
	void add_walk_anim_counter(int amount);
	void add_death_anim(int amount);

	void set_is_dying(bool new_is_dying);
	void set_walk_anim_counter(int new_player_walk_anim_counter);
	void set_death_anim(int new_player_death_anim);
	void set_right_foot(bool state);
	
	void draw(Texture2D player_character_spritesheet);
	void draw_death(Texture2D player_character_death);

	void move();
	void be_attacked(Sound player_death, std::vector<Enemy>& enemy_tracker, std::vector<Enemy>& enemy_pool, int& active_enemies, std::vector<PowerUp>& powerUp_tracker, std::vector<PowerUp>& powerUp_pool, std::vector<PowerUp>& bullet_tracker, std::vector<PowerUp>& bullet_pool);
	void be_restrained(float x, float y);
	void collide(std::vector<Obstacle>& obstacle_tracker);
	void collect(std::vector<PowerUp>& powerUp_tracker, std::vector<PowerUp>& powerUp_pool, Sound power_up_pick_up, Sound coin_sound);
	void shoot(Sound shoot_fx);

	void bullet_update();
	void bullet_attack();
	void bullet_collision();
	void bullet_draw(Texture2D bullet_player);
};