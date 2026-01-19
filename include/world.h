#pragma once

#define CHUNK_SIZE 16
#define CHUNK_VOLUME (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)

#include <cglm/cglm.h>
#include <memory.h>
#include <buffers.h>

typedef enum block_type_t {
    Air,
    Dirt
} block_type_t;

typedef struct block_t {
    block_type_t block_type;
} block_t;

typedef struct chunk_t {
    block_t blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
    ivec3 world_pos;
} chunk_t;

void chunk_init(chunk_t* chunk, ivec3 pos);
void chunk_generate(chunk_t* chunk);
void chunk_render(chunk_t* chunk, vertex_buffer_t* vb);