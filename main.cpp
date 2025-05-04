#include <raylib.h>
#include <resource_dir.h>

#include "map.h"
#include "entity.h"

int main ()
{
	const float tile_size = 32;
	const float area_size = tile_size * 16;

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(area_size + (tile_size * 3), area_size + tile_size, "Journey of The Prairie King");
	SearchAndSetResourceDir("resources");
	SetTargetFPS(60);

	bool game_started = false;
	bool zoom_completed = false;
	
	std::vector<Desert> desert_areas = { Desert("AREAS/area1_1.txt"), Desert("AREAS/area1_2.txt"), Desert("AREAS/area1_3.txt") };

	int level_count = 0;
	Map active_map;
	active_map = desert_areas[level_count];
	level_count++;

	Player player(tile_size, area_size);
	player.reset_health();
	player.reset_position(tile_size, area_size);

	InitAudioDevice();
	Audio audio;
	audio.play_music();
	
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
