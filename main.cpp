#include "raylib.h"
#include "world.h"
#include "world_generation.h"
#include "meshing.h"
#include "player.h"
#include "raymath.h"
#include <vector>
#include <cstdlib>
#include <unordered_map>

int main(void)
{
    const int screen_width = 1280;
    const int screen_height = 720;

    InitWindow(screen_width, screen_height, "Voxel World");

    World world;
    init_world(&world);

    Player player;
    init_player(&player);

    std::unordered_map<long long, Mesh> meshes;

    const int render_distance = 8;
    for (int x = 0; x < render_distance; x++) {
        for (int z = 0; z < render_distance; z++) {
            int chunk_x = x - render_distance / 2;
            int chunk_z = z - render_distance / 2;
            Chunk* chunk = (Chunk*)malloc(sizeof(Chunk));
            generate_chunk(chunk, chunk_x, chunk_z);
            world.chunks[world_chunk_key(chunk_x, chunk_z)] = chunk;
        }
    }

    bool cursor_disabled = true;
    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_M)) {
            cursor_disabled = !cursor_disabled;
            if (cursor_disabled) {
                DisableCursor();
            } else {
                EnableCursor();
            }
        }

        update_player(&player, &world, GetFrameTime(), cursor_disabled);

        for (auto const& [key, chunk] : world.chunks) {
            if (chunk->is_dirty) {
                if (meshes.count(key)) {
                    UnloadMesh(meshes[key]);
                }
                meshes[key] = generate_chunk_mesh(chunk, &world);
                chunk->is_dirty = false;
                break;
            }
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(player.camera);

        for (auto const& [key, mesh] : meshes) {
            DrawMesh(mesh, LoadMaterialDefault(), MatrixIdentity());
        }

        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    for (auto const& [key, mesh] : meshes) {
        UnloadMesh(mesh);
    }
    meshes.clear();

    destroy_world(&world);
    CloseWindow();

    return 0;
}