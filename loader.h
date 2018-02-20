#ifndef _LOADER_H
#define _LOADER_H

#include "redismodule.h"
#include "redisassert.h"

#define malloc(size) RedisModule_Alloc(size)
#define calloc(count, size) RedisModule_Calloc(count, size)
#define realloc(ptr, size) RedisModule_Realloc(ptr, size)
#define free(ptr) RedisModule_Free(ptr)

int ModuleGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int ModuleSet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);

int TmemGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemPut(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemInval(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemGenerateSize(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);

int TmemDummy(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemReal(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemSleepy(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemAwake(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemSilent(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemAnswer(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemGenerate(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemInput(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);

/* TmemSilent renamed to use the name for a control operation */
int TmemRedisSilent(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemSilentDirty(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemRedisGenerate(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);


int TmemDrop(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemPoison(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemPoisonMalloc(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemEcho(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);


int TmemFileGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemFilePut(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);


struct chunk_key {
    uint16_t chunk_id;
    uint16_t index;
};


#endif /* _LOADER_H */
