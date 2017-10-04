#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "redismodule.h"
#include "tmem.h"
#include "chunk.h"

#define TMEM_PATH ("/dev/tmem_dev")

int fd;

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {


    if (RedisModule_Init(ctx, "tmem", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "tmem.dummy", TmemDummy, "write", 0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "tmem.get", TmemGet, "write", 0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "tmem.put", TmemPut, "write", 0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "tmem.fileget", TmemFileGet, "write", 0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "tmem.fileput", TmemFilePut, "write", 0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "tmem.inval", TmemInval, "write", 0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;
    
    if (RedisModule_CreateCommand(ctx, "tmem.chunkfill", ChunkFill, "write", 0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "tmem.chunkkey", ChunkKey, "write", 0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "tmem.chunkinmemory", ChunkInMemory, "write", 0, 0, 0) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    fd = open(TMEM_PATH, O_RDWR); 
    if (!fd)
        return REDISMODULE_ERR;
   
    return REDISMODULE_OK;
}


