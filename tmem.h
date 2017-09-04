#ifndef _TMEM_H
#define _TMEM_H

#include "redismodule.h"
#include "redisassert.h"

#define malloc(size) RedisModule_Alloc(size)
#define calloc(count, size) RedisModule_Calloc(count, size)
#define realloc(ptr, size) RedisModule_Realloc(ptr, size)
#define free(ptr) RedisModule_Free(ptr)

int TmemGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemPut(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemInval(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);

struct chunk_key {
    uint16_t chunk_id;
    uint16_t index;
};


#endif /* _TMEM_H */
