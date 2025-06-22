#pragma once

#include "chunk.h"

struct World;

Mesh generate_chunk_mesh(Chunk* chunk, World* world);