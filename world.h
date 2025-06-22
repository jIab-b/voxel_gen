#pragma once

#include "chunk.h"
#include <unordered_map>

// Combine two 32-bit integers into one 64-bit integer.
inline long long world_chunk_key(int x, int z) {
    return ((long long)x << 32) | (unsigned int)z;
}

struct World {
    std::unordered_map<long long, Chunk*> chunks;
};

void init_world(World* world);
void destroy_world(World* world);
Chunk* get_chunk(World* world, int x, int z);
void set_voxel(World* world, int x, int y, int z, Voxel voxel);
Voxel get_voxel(World* world, int x, int y, int z);