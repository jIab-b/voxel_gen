#pragma once

typedef enum {
    VOXEL_TYPE_AIR = 0,
    VOXEL_TYPE_DIRT,
    VOXEL_TYPE_GRASS,
    VOXEL_TYPE_STONE,
    VOXEL_TYPE_WATER
} VoxelType;

typedef struct {
    VoxelType type;
    unsigned char data;
} Voxel;