#include "world.h"
#include <stdlib.h>

void init_world(World* world) {
    world->chunks.clear();
}

void destroy_world(World* world) {
    for (auto const& [key, val] : world->chunks) {
        free(val);
    }
    world->chunks.clear();
}

Chunk* get_chunk(World* world, int x, int z) {
    long long key = world_chunk_key(x, z);
    if (world->chunks.count(key)) {
        return world->chunks[key];
    }
    return NULL;
}

void set_voxel(World* world, int x, int y, int z, Voxel voxel) {
    int chunk_x = x / CHUNK_WIDTH;
    int chunk_z = z / CHUNK_DEPTH;
    int local_x = x % CHUNK_WIDTH;
    if (local_x < 0) {
        local_x += CHUNK_WIDTH;
        chunk_x--;
    }
    int local_y = y;
    int local_z = z % CHUNK_DEPTH;
    if (local_z < 0) {
        local_z += CHUNK_DEPTH;
        chunk_z--;
    }

    Chunk* chunk = get_chunk(world, chunk_x, chunk_z);
    if (chunk != NULL) {
        chunk->blocks[local_x][local_y][local_z] = voxel;
        chunk->is_dirty = true;
    }
}

Voxel get_voxel(World* world, int x, int y, int z) {
    int chunk_x = x / CHUNK_WIDTH;
    int chunk_z = z / CHUNK_DEPTH;
    int local_x = x % CHUNK_WIDTH;
    if (local_x < 0) {
        local_x += CHUNK_WIDTH;
        chunk_x--;
    }
    int local_y = y;
    int local_z = z % CHUNK_DEPTH;
    if (local_z < 0) {
        local_z += CHUNK_DEPTH;
        chunk_z--;
    }

    Chunk* chunk = get_chunk(world, chunk_x, chunk_z);
    if (chunk != NULL) {
        return chunk->blocks[local_x][local_y][local_z];
    }

    Voxel air;
    air.type = VOXEL_TYPE_AIR;
    return air;
}