#ifndef _CHUNK_H
#define _CHUNK_H

#include "redismodule.h"
#include "tmem.h"

#define CHUNK_SIZE (8192)
#define MAX_MEMBERS (100)

struct chunk {
    uint16_t id;
    uint16_t cur_size;
    uint16_t indices[MAX_MEMBERS];
    uint16_t cur_index;
    char contents[CHUNK_SIZE - (MAX_MEMBERS + 2)*sizeof(uint16_t)];
};

struct chunk *createChunk();
void freeChunk(struct chunk *chunk);
uint16_t addPair(struct chunk *chunk, void *data, size_t len);

int ChunkFill(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int ChunkKey(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int ChunkInMemory(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);

#define ADDPAIR_ERROR (0xFFFF)

#endif /* _CHUNK_H */
