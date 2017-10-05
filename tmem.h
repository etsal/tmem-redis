#ifndef _TMEM_H
#define _TMEM_H

#include "redismodule.h"
#include "redisassert.h"

#define malloc(size) RedisModule_Alloc(size)
#define calloc(count, size) RedisModule_Calloc(count, size)
#define realloc(ptr, size) RedisModule_Realloc(ptr, size)
#define free(ptr) RedisModule_Free(ptr)

int TmemDummy(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);

int ModuleGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int ModuleSet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);

int TmemGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemPut(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemInval(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);


int TmemSilent(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
/* SilentGetDirty exists just to prove that it's not a page cache problem */
int TmemSilentDirty(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemGenerate(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);


/* Ignore and drop exist just to prove that referencing strings is cheap */
int TmemIgnore(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemDrop(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemPoison(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemEcho(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);


int TmemFileGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
int TmemFilePut(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);


struct chunk_key {
    uint16_t chunk_id;
    uint16_t index;
};


#endif /* _TMEM_H */
