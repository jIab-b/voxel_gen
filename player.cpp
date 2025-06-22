#include "player.h"
#include "raymath.h"

void init_player(Player* player) {
    player->camera.position = Vector3{ 0.0f, 35.0f, 10.0f };
    player->camera.target = Vector3{ 0.0f, 35.0f, 0.0f };
    player->camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    player->camera.fovy = 45.0f;
    player->camera.projection = CAMERA_PERSPECTIVE;
    player->velocity = Vector3Zero();
    player->is_on_ground = false;
}

void update_player(Player* player, World* world, float delta_time, bool cursor_disabled) {
    // Mouse look
    if (cursor_disabled) {
        UpdateCamera(&player->camera, CAMERA_FIRST_PERSON);
    }

    // Movement
    Vector3 move = {0};
    if (IsKeyDown(KEY_W)) move.z -= 1.0f;
    if (IsKeyDown(KEY_S)) move.z += 1.0f;
    if (IsKeyDown(KEY_A)) move.x -= 1.0f;
    if (IsKeyDown(KEY_D)) move.x += 1.0f;

    // Jump
    if (IsKeyPressed(KEY_SPACE) && player->is_on_ground) {
        player->velocity.y = 10.0f;
        player->is_on_ground = false;
    }

    // Apply gravity
    if (!player->is_on_ground) {
        player->velocity.y -= 20.0f * delta_time;
    }

    // Apply movement
    Matrix camera_matrix = GetCameraMatrix(player->camera);
    Vector3 forward = Vector3Transform(Vector3{ 0, 0, -1 }, camera_matrix);
    Vector3 right = Vector3Transform(Vector3{ 1, 0, 0 }, camera_matrix);

    player->camera.position = Vector3Add(player->camera.position, Vector3Scale(forward, move.z * 5.0f * delta_time));
    player->camera.position = Vector3Add(player->camera.position, Vector3Scale(right, move.x * 5.0f * delta_time));
    player->camera.position = Vector3Add(player->camera.position, Vector3Scale(player->velocity, delta_time));

    // Collision detection
    Voxel voxel_below = get_voxel(world, (int)player->camera.position.x, (int)player->camera.position.y - 2, (int)player->camera.position.z);
    if (voxel_below.type != VOXEL_TYPE_AIR) {
        player->is_on_ground = true;
        player->velocity.y = 0;
        player->camera.position.y = (int)player->camera.position.y;
    } else {
        player->is_on_ground = false;
    }
}