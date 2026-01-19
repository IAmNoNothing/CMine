#include "world.h"

void chunk_init(chunk_t *chunk, ivec3 pos)
{
    memcpy(chunk->world_pos, pos, sizeof(ivec3));
}

void chunk_generate(chunk_t *chunk)
{
    memset(chunk->blocks, (int)Dirt, CHUNK_VOLUME);
}

void chunk_render(chunk_t *chunk, vertex_buffer_t *vb)
{
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                
            }
        }
    }
}
