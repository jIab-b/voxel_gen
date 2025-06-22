#pragma once

#include "raylib.h"
#include "world.h"

#define PLAYER_SPAWN_Y 35.0f
#define PLAYER_JUMP_HEIGHT 10.0f
#define PLAYER_GRAVITY 20.0f
#define PLAYER_SPEED 5.5f
#define MOUSE_SENSITIVITY 0.003f
#define PLAYER_GROUND_CHECK_DISTANCE 2
typedef struct {
    Camera camera;
    Vector3 velocity;
    bool is_on_ground;
} Player;

void init_player(Player* player);
void update_player(Player* player, World* world, float delta_time, bool cursor_disabled);