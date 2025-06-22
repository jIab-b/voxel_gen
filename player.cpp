#include "player.h"
#include "raymath.h"

void init_player(Player* player) {
    player->camera.position = Vector3{ 0.0f, PLAYER_SPAWN_Y, 10.0f };
    player->camera.target = Vector3{ 0.0f, PLAYER_SPAWN_Y, 0.0f };
    player->camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    player->camera.fovy = 45.0f;
    player->camera.projection = CAMERA_PERSPECTIVE;
    player->velocity = Vector3Zero();
    player->is_on_ground = false;
}

void update_player(Player* player, World* world, float delta_time, bool cursor_disabled) {
    // Mouse look
    if (cursor_disabled) {
        Vector2 mouse_delta = GetMouseDelta();
        float yaw_delta = -mouse_delta.x * MOUSE_SENSITIVITY;
        float pitch_delta = -mouse_delta.y * MOUSE_SENSITIVITY;

        Vector3 view_vector = Vector3Subtract(player->camera.target, player->camera.position);

        // Yaw rotation
        view_vector = Vector3RotateByAxisAngle(view_vector, player->camera.up, yaw_delta);

        // Pitch rotation and clamping
        Vector3 right_vector = Vector3CrossProduct(view_vector, player->camera.up);
        right_vector = Vector3Normalize(right_vector);
        
        float current_pitch = asinf(view_vector.y / Vector3Length(view_vector));
        float pitch_limit = 1.5f; // ~85 degrees

        float desired_pitch = current_pitch + pitch_delta;
        if (desired_pitch > pitch_limit) {
            pitch_delta = pitch_limit - current_pitch;
        } else if (desired_pitch < -pitch_limit) {
            pitch_delta = -pitch_limit - current_pitch;
        }
        
        view_vector = Vector3RotateByAxisAngle(view_vector, right_vector, pitch_delta);
        player->camera.target = Vector3Add(player->camera.position, view_vector);
    }

    // Player input
    Vector3 move_input = Vector3Zero();
    if (IsKeyDown(KEY_W)) move_input.z += 1.0f;
    if (IsKeyDown(KEY_S)) move_input.z -= 1.0f;
    if (IsKeyDown(KEY_A)) move_input.x -= 1.0f;
    if (IsKeyDown(KEY_D)) move_input.x += 1.0f;

    // Jump
    if (IsKeyPressed(KEY_SPACE) && player->is_on_ground) {
        player->velocity.y = PLAYER_JUMP_HEIGHT;
        player->is_on_ground = false;
    }

    // Apply gravity
    if (!player->is_on_ground) {
        player->velocity.y -= PLAYER_GRAVITY * delta_time;
    }

    // Calculate movement vectors based on horizontal view
    Vector3 forward = Vector3Subtract(player->camera.target, player->camera.position);
    forward.y = 0;
    forward = Vector3Normalize(forward);

    Vector3 right = Vector3CrossProduct(forward, player->camera.up);
    right = Vector3Normalize(right);

    // Calculate total displacement for this frame
    Vector3 horizontal_displacement = Vector3Add(
        Vector3Scale(forward, move_input.z * PLAYER_SPEED * delta_time),
        Vector3Scale(right, move_input.x * PLAYER_SPEED * delta_time)
    );
    Vector3 vertical_displacement = Vector3Scale(player->velocity, delta_time);
    Vector3 total_displacement = Vector3Add(horizontal_displacement, vertical_displacement);

    // Apply displacement to both camera position and target
    player->camera.position = Vector3Add(player->camera.position, total_displacement);
    player->camera.target = Vector3Add(player->camera.target, total_displacement);

    // Collision detection and response
    Voxel voxel_below = get_voxel(world, (int)player->camera.position.x, (int)player->camera.position.y - PLAYER_GROUND_CHECK_DISTANCE, (int)player->camera.position.z);
    if (voxel_below.type != VOXEL_TYPE_AIR) {
        player->is_on_ground = true;
        player->velocity.y = 0;

        // Snap player to ground and adjust target accordingly
        float ground_y = (float)((int)(player->camera.position.y - PLAYER_GROUND_CHECK_DISTANCE)) + 2.0f;
        float y_diff = ground_y - player->camera.position.y;
        player->camera.position.y += y_diff;
        player->camera.target.y += y_diff;
    } else {
        player->is_on_ground = false;
    }
}