#include "world_generation.h"
#include "glm/gtc/noise.hpp"

void generate_chunk(Chunk* chunk, int chunk_x, int chunk_z) {
    chunk->position = Vector3{(float)chunk_x * CHUNK_WIDTH, 0, (float)chunk_z * CHUNK_DEPTH};
    chunk->is_dirty = true;

    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int z = 0; z < CHUNK_DEPTH; z++) {
            float world_x = (float)(chunk_x * CHUNK_WIDTH + x);
            float world_z = (float)(chunk_z * CHUNK_DEPTH + z);

            float noise_val = glm::perlin(glm::vec2(world_x / 64.0f, world_z / 64.0f));
            int height = (int)((noise_val + 1.0f) / 2.0f * (CHUNK_HEIGHT - 1));

            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                if (y > height) {
                    chunk->blocks[x][y][z].type = VOXEL_TYPE_AIR;
                } else if (y == height) {
                    chunk->blocks[x][y][z].type = VOXEL_TYPE_GRASS;
                } else if (y > height - 5) {
                    chunk->blocks[x][y][z].type = VOXEL_TYPE_DIRT;
                } else {
                    chunk->blocks[x][y][z].type = VOXEL_TYPE_STONE;
                }
            }
        }
    }
}