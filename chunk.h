#pragma once

#include "voxel.h"
#include "raylib.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 32
#define CHUNK_DEPTH 16

typedef struct {
    Voxel blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
    Vector3 position;
    bool is_dirty;
} Chunk;