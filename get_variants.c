#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>


#include "redismodule.h"
#include "tmem.h"
#include "tmem_ioctl.h"

/* Like TmemGet, but instead of a tmem operation generate the string here */
int TmemRedisGenerate(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    int ret; 
    size_t key_len, value_len;
    char *value ;

    if (argc != 2)
        return RedisModule_WrongArity(ctx);

    if (RedisModule_StringToLongLong(argv[1], &value_len) == REDISMODULE_ERR) {
        RedisModule_ReplyWithSimpleString(ctx, "ERROR"); 
        return REDISMODULE_ERR;
    }

    value = malloc(value_len);
    if (!value) {
        RedisModule_ReplyWithSimpleString(ctx, "ERROR"); 
        return REDISMODULE_OK;
    }
    
    memset(value, 0x61, value_len);

    if (ret) 
        RedisModule_ReplyWithSimpleString(ctx, "ERROR"); 
    else
        RedisModule_ReplyWithStringBuffer(ctx, value, value_len);
    
    free(value);

    return REDISMODULE_OK;
    
 
}

/* Like TmemGet, but do not return the string to the client */
int TmemRedisSilent(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    int ret; 
    size_t key_len, value_len;
    char *value ;
    char *key;

    if (argc != 2)
        return RedisModule_WrongArity(ctx);

    key = RedisModule_StringPtrLen(argv[1], &key_len);    
    value = malloc(TMEM_MAX);
    if (!value) {
        RedisModule_ReplyWithSimpleString(ctx, "ERROR"); 
        return REDISMODULE_OK;
    }
    
    ret = tmem_ioctl_get(key, key_len, value, &value_len, 0);

    if (ret) 
        RedisModule_ReplyWithSimpleString(ctx, "ERROR"); 
    else
        RedisModule_ReplyWithSimpleString(ctx, "OK");
    
    free(value);

    return REDISMODULE_OK;
}    

/* Like TmemSilent, but touch all the pages before returning */
int TmemSilentDirty(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    int ret; 
    size_t key_len, value_len;
    char *value ;
    char *key;
    int i;

    if (argc != 2)
        return RedisModule_WrongArity(ctx);

    key = RedisModule_StringPtrLen(argv[1], &key_len);    
    value = malloc(TMEM_MAX);
    if (!value) {
        RedisModule_ReplyWithSimpleString(ctx, "ERROR"); 
        return REDISMODULE_OK;
    }
    
    ret = tmem_ioctl_get(key, key_len, value, &value_len, 0);

    for (i = 0; i < value_len; i += PAGE_SIZE)
	value[i] = 'a'; 

    if (ret) 
        RedisModule_ReplyWithSimpleString(ctx, "ERROR"); 
    else
        RedisModule_ReplyWithSimpleString(ctx, "OK");
    
    free(value);

    return REDISMODULE_OK;
}
