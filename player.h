#pragma once

#include "raylib.h"
#include "world.h"

typedef struct {
    Camera camera;
    Vector3 velocity;
    bool is_on_ground;
} Player;

void init_player(Player* player);
void update_player(Player* player, World* world, float delta_time, bool cursor_disabled);