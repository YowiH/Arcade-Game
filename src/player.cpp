#include "player.h"
#include "enemy.h"
#include "obstacle.h"
#include "powerUp.h"

Player::Player() : Walker(rec) {
	set_position({ left_margin + (area_size / 2), tile_size + (area_size * 3 / 4) });
	hp = 3;
	speed = 2;
	damage = 1;
}

bool Player::get_is_dying() {
	return is_dying;
}

int Player::get_coins() {
	return coins;
}

int Player::get_mov_dir() {
	return mov_dir;
}

int Player::get_walk_anim_counter() {
	return walk_anim_counter;
}

bool Player::get_right_foot() {
	return right_foot;
}

int Player::get_damage() {
	return damage;
}

void Player::add_coins(int amount) {
	coins += amount;
}

void Player::add_walk_anim_counter(int amount) {
	walk_anim_counter += amount;
}

void Player::add_death_anim(int amount) {
	death_anim += amount;
}

void Player::set_is_dying(bool new_state) {
	is_dying = new_state;
}

void Player::set_walk_anim_counter(int new_amount) {
	walk_anim_counter = new_amount;
}

void Player::set_death_anim(int new_amount) {
	death_anim = new_amount;
}

void Player::set_right_foot(bool new_state) {
	right_foot = new_state;
}

void Player::draw(Texture2D player_character_spritesheet) {
	Rectangle src;
	if (anim_dir != 0) {//esta disparant
		if (mov_dir != 0) { //s'esta movent
			switch (anim_dir) {
			case 1: //dreta
				if (right_foot) {
					src = { 16 * 6, 0, 16, 16 };
				}
				else {
					src = { 0, 16, 16, 16 };
				}
				break;
			case 2: //avall
				if (right_foot) {
					src = { 16 * 4, 0, 16, 16 };
				}
				else {
					src = { 16 * 3, 0, 16, 16 };
				}
				break;
			case 3: //esquerra
				if (right_foot) {
					src = { 16 * 6, 16, 16, 16 };
				}
				else {
					src = { 16 * 5, 16, 16, 16 };
				}
				break;
			case 4: //amunt 
				if (right_foot) {
					src = { 16 * 2, 16, 16, 16 };
				}
				else {
					src = { 16 * 3, 16, 16, 16 };
				}
				break;
			case 0:
				src = { 0, 0, 16, 16 };
				break;
			}
			DrawTexturePro(player_character_spritesheet, src, rec, { 0, 0 }, 0, WHITE);
		}
		else {//no s'esta movent
			switch (anim_dir) {
			case 1: //dreta
				src = { 16 * 5, 0, 16, 16 };
				break;
			case 2: //avall
				src = { 16 * 2, 0, 16, 16 };
				break;
			case 3: //esquerra
				src = { 16 * 4, 16, 16, 16 };
				break;
			case 4: //amunt 
				src = { 16 , 16, 16, 16 };
				break;
			}
			DrawTexturePro(player_character_spritesheet, src, rec, { 0, 0 }, 0, WHITE);

		}
	}
	else {//no esta disparant
		switch (mov_dir) {//s'està movent
		case 1: //dreta
			if (right_foot) {
				src = { 16 * 6, 0, 16, 16 };
			}
			else {
				src = { 0, 16, 16, 16 };
			}
			break;
		case 2: //avall
			if (right_foot) {
				src = { 16 * 4, 0, 16, 16 };
			}
			else {
				src = { 16 * 3, 0, 16, 16 };
			}
			break;
		case 3: //esquerra
			if (right_foot) {
				src = { 16 * 6, 16, 16, 16 };
			}
			else {
				src = { 16 * 5, 16, 16, 16 };
			}
			break;
		case 4: //amunt 
			if (right_foot) {
				src = { 16 * 2, 16, 16, 16 };
			}
			else {
				src = { 16 * 3, 16, 16, 16 };
			}
			break;
		case 0: //no es mou ni dispara
			src = { 0, 0, 16, 16 };
			break;
		}
		DrawTexturePro(player_character_spritesheet, src, rec, { 0, 0 }, 0, WHITE);
	}

	shoot_dir = { 0,0 };
	anim_dir = 0;
	mov_dir = 0;

}

void Player::draw_death(Texture2D player_character_death) {
	//player_walk_anim_counter++;
	//actualitzar animacions
	Rectangle src;
	if (death_anim < 10) {
		src = { 0, 0, 16, 16 };
	}
	else if (death_anim < 10 * 2) {
		src = { 16 * 1, 0, 16, 16 };
	}
	else if (death_anim < 10 * 3) {
		src = { 16 * 2, 0, 16, 16 };
	}
	else if (death_anim < 10 * 4) {
		src = { 16 * 3, 0, 16, 16 };
	}
	else if (death_anim <= 10 * 10) {}
	else {
		is_dying = false;
		set_position({ left_margin + (area_size / 2), tile_size + (area_size * 3 / 4) });
		walk_anim_counter = 0;
		death_anim = 0;
	}
	//dibuixa si han passat menys de 40 frames 
	if (death_anim < 10 * 4) {
		//waits
		DrawTexturePro(player_character_death, src, rec, { 0,0 }, 0, WHITE);
	}
	//DrawTexturePro(player_character_death, src, { player_pos.x, player_pos.y, tile_size, tile_size }, { 0,0 }, 0, WHITE);
}

void Player::move() {
	if (IsKeyDown('A') && !xNegBlock) {
		//player_pos = { player_pos.x - player_Speed, player_pos.y };
		player_mov_dir.x = -1;
		mov_dir = 3;

	}
	else if (IsKeyDown('D') && !xPosBlock) {
		//player_pos = { player_pos.x + player_Speed, player_pos.y };
		player_mov_dir.x = 1;
		mov_dir = 1;

	}

	if (IsKeyDown('S') && !yPosBlock) {
		//player_pos = { player_pos.x, player_pos.y + player_Speed };
		player_mov_dir.y = 1;
		mov_dir = 2;

	}
	else if (IsKeyDown('W') && !yNegBlock) {
		//player_pos = { player_pos.x, player_pos.y - player_Speed };
		player_mov_dir.y = -1;
		mov_dir = 4;

	}

	//in order to make diagonal movements as fast as horizontal and vertical movements we must reduce the velocity in both directions
	if (player_mov_dir.y != 0 && player_mov_dir.x != 0) {
		set_position({ rec.x + player_mov_dir.x * speed * 0.707f, rec.y + player_mov_dir.y * speed * 0.707f });
	}
	else {
		set_position({rec.x + player_mov_dir.x * speed, rec.y + player_mov_dir.y * speed});
	}

	//x axis limits
	if (rec.x > area_size + (tile_size)) {
		set_position({ area_size + (tile_size), rec.y });
	}
	else if (rec.x < (tile_size * 4)) {
		set_position({ (tile_size * 4), rec.y });
	}
	//y axis limits
	if (rec.y > area_size - tile_size) {
		set_position({ rec.x, area_size - tile_size });
	}
	else if (rec.y < tile_size * 2) {
		set_position({ rec.x, tile_size * 2 });
	}
	player_mov_dir = { 0, 0 };
	//restart block variables
	xPosBlock = false;
	xNegBlock = false;
	yPosBlock = false;
	yNegBlock = false;
}

void Player::be_attacked(Sound player_death, std::vector<Enemy>& enemy_tracker, std::vector<Enemy>& enemy_pool, int& active_enemies, std::vector<PowerUp>& powerUp_tracker, std::vector<PowerUp>& powerUp_pool, std::vector<Bullet>& bullet_tracker, std::vector<Bullet>& bullet_pool) {
	bool already_damaged = false;
	int i = 0;
	while (!already_damaged && i < enemy_tracker.size()) {
		if (CheckCollisionCircles({ rec.x + (rec.x / 2), rec.y + (rec.height / 2) }, rec.width / 2, { enemy_tracker[i].get_rec().x + (tile_size / 2), enemy_tracker[i].get_rec().y + (tile_size / 2) }, tile_size / 2)) {
			already_damaged = true;
			hp--;

			//player death
			is_dying = true;
			death_anim = 0;
			PlaySound(player_death);
			//borrar enemics
			for (int i = enemy_tracker.size() - 1; i >= 0; i--) {
				//save the bullet in the pool
				enemy_pool.push_back(std::move(enemy_tracker[i]));
				//borrar bullet
				auto& j = enemy_tracker.begin() + i;
				enemy_tracker.erase(j);

			}
			active_enemies = 0;
			//borrar totes les bales
			for (int i = bullet_tracker.size() - 1; i >= 0; i--) {
				//save the bullet in the pool
				bullet_pool.push_back(std::move(bullet_tracker[i]));
				//borrar bullet
				auto& j = bullet_tracker.begin() + i;
				bullet_tracker.erase(j);

			}
			//borrar tots els power ups
			for (int i = powerUp_tracker.size() - 1; i >= 0; i--) {
				//save the bullet in the pool
				powerUp_pool.push_back(std::move(powerUp_tracker[i]));
				//borrar bullet
				auto& j = powerUp_tracker.begin() + i;
				powerUp_tracker.erase(j);

			}

			if (hp < 0) {
				std::cout << "you are dead" << std::endl;
			}
		}
		i++;
	}
}

float GetAngle(Vector2 v, Vector2 u) {
	float c = (u.x * v.x + u.y * v.y); //prop al cosinus
	float s = (u.x * v.y - u.y * v.x); //prop al sinus

	return atan2(s, c) + PI;
}

void Player::be_restrained(float x, float y) {

	//define two vectors and use them to conclude the direction of the colision
	Vector2 v = { 0, 0 };
	Vector2 u = { 0, 0 };

	//inici vector
	float x0 = rec.x;
	float y0 = rec.y;

	//v va des del centre del obstacle (cantonada esquerra de dalt) al centre del jugador
	v = { x0 - x, y0 - y };
	//u va des del centre del jugador al centre de l'obstacle
	u = { tile_size / 2, 0 };
	float theta = GetAngle(u, v);

	if (theta >= PI / 4 && theta < 3 * PI / 4) {
		//colisio per dalt
		yNegBlock = true;
	}
	else if (theta >= 3 * PI / 4 && theta < 5 * PI / 4) {
		//colisió per l'esquerra
		xNegBlock = true;
	}
	else if (theta >= 5 * PI / 4 && theta < 7 * PI / 4) {
		//colisio per baix
		yPosBlock = true;
	}
	else if (theta >= 7 * PI / 4 || theta < PI / 4) {
		//colisio per la dreta
		xPosBlock = true;
	}
	else {
		std::cout << "error when calculating the angle" << std::endl;
	}

	return;

}

void Player::collide(std::vector<Obstacle>& obstacle_tracker) {
	//check all obstacle colliders
	Rectangle rec_player = { rec.x, rec.y, tile_size, tile_size };
	for (int i = 0; i < obstacle_tracker.size(); i++) {
		if (CheckCollisionRecs(rec_player, obstacle_tracker[i].get_rec())) {
			be_restrained(obstacle_tracker[i].get_rec().x, obstacle_tracker[i].get_rec().y);
		}
	}
}


void Player::collect(std::vector<PowerUp>& powerUp_tracker, std::vector<PowerUp>& powerUp_pool, Sound power_up_pick_up, Sound coin_sound) {
	for (int i = powerUp_tracker.size() - 1; i >= 0; i--) {
		if (CheckCollisionCircles({ rec.x + (rec.width / 2), rec.y + (rec.height / 2) }, rec.width / 2, { powerUp_tracker[i].get_position().x + tile_size / 2, powerUp_tracker[i].get_position().y + tile_size / 2}, tile_size / 4)) {
			switch (powerUp_tracker[i].get_type()) {
			case 'U': //vida extra
				hp++;
				PlaySound(power_up_pick_up);
				break;
			case 'O'://monedes
				coins++;
				PlaySound(coin_sound);
				break;
			}
			//guardar power up al pool
			powerUp_pool.push_back(std::move(powerUp_tracker[i]));
			//borrar powerUp
			auto& k = powerUp_tracker.begin() + i;
			powerUp_tracker.erase(k);
		}
	}
}

void Player::shoot(Sound shoot_fx, std::vector<Bullet>& bullet_tracker, std::vector<Bullet>& bullet_pool) {
	shoot_dir = { 0, 0 };
	//we get the direction of the bullet
	if (IsKeyDown(KEY_UP)) {
		shoot_dir = { shoot_dir.x, -1 };
		anim_dir = 4;
	}
	else if (IsKeyDown(KEY_DOWN)) {
		shoot_dir = { shoot_dir.x, 1 };
		anim_dir = 2;
	}
	if (IsKeyDown(KEY_RIGHT)) {
		shoot_dir = { 1, shoot_dir.y };
		anim_dir = 1;
	}
	else if (IsKeyDown(KEY_LEFT)) {
		shoot_dir = { -1, shoot_dir.y };
		anim_dir = 3;
	}

	//create bullets
	if ((shoot_dir.x != 0 || shoot_dir.y != 0) && fire_frame_counter % fire_rate == 0) {
		//first look for bullets in the pool
		PlaySound(shoot_fx);
		if (bullet_pool.empty()) {
			Bullet bullet({ rec.x, rec.y });
			bullet.set_damage(damage);
			bullet.set_velocity(shoot_dir);
			bullet_tracker.push_back(std::move(bullet));
		}
		else {
			bullet_tracker.push_back(std::move(bullet_pool.back()));
			bullet_tracker.back().set_damage(damage);
			bullet_tracker.back().set_position({ rec.x + tile_size / 4 , rec.y + tile_size / 4 });
			bullet_tracker.back().set_velocity(shoot_dir);
			bullet_pool.pop_back();
		}
		fire_frame_counter++;
	}
	else if (fire_frame_counter % fire_rate != 0) {
		fire_frame_counter++;
	}
	//shoot_dir = { 0, 0 };
}