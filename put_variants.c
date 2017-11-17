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

extern int fd;


/* Like TmemPut, but return the value to the client */
int TmemEcho(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
    int ret;
    size_t key_len, value_len;
    char *key, *value;

    if (argc != 3)
        return RedisModule_WrongArity(ctx);

    key = RedisModule_StringPtrLen(argv[1], &key_len);
    value = RedisModule_StringPtrLen(argv[2], &value_len);
    ret = tmem_ioctl_put(key, key_len, value, value_len);

    if (ret)
        RedisModule_ReplyWithSimpleString(ctx, "ERROR");
    else 
	RedisModule_ReplyWithStringBuffer(ctx, value, value_len);

    return REDISMODULE_OK;
}


/* Like TmemPut, but just drop the request */
int TmemDrop(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    

    size_t key_len, value_len;
    char *key, *value;
   
    if (argc != 3)
        return RedisModule_WrongArity(ctx);

    key = RedisModule_StringPtrLen(argv[1], &key_len);
    value = RedisModule_StringPtrLen(argv[2], &value_len); 

    RedisModule_ReplyWithSimpleString(ctx, "OK");

    return REDISMODULE_OK;
}


/* Like TmemPut, but accepts no input - it poisons the entry by filling it with a specific byte */
int TmemPoison(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
    size_t key_len, value_len;
    long long times_poisoned;
    char *key, *value; 
    int ret;
    int i;

    if (argc != 4)
	return RedisModule_WrongArity(ctx);

    key = RedisModule_StringPtrLen(argv[1], &key_len);

    if (RedisModule_StringToLongLong(argv[2], &value_len) == REDISMODULE_ERR) {
	RedisModule_ReplyWithSimpleString(ctx, "ERROR");
	return REDISMODULE_ERR;
    }

    if (RedisModule_StringToLongLong(argv[3], &times_poisoned) == REDISMODULE_ERR) {
	RedisModule_ReplyWithSimpleString(ctx, "ERROR");	
	return REDISMODULE_ERR;
    }

    value = malloc(value_len);
    if (!value) { 
	RedisModule_ReplyWithSimpleString(ctx, "ERROR");
	return REDISMODULE_OK;
    }
    
    memset(value, 0xfe, value_len);

    for (i = 0; i < times_poisoned; i++) {	
	ret = tmem_ioctl_put(key, key_len, value, value_len);
	if (ret)
	    break;
    }
    
    free(value);

    RedisModule_ReplyWithSimpleString(ctx, "OK");

    return REDISMODULE_OK;
}

/* Like TmemPoison, but also repeats the allocation, setting, and freeing of the poison value */
int TmemPoisonMalloc(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    

    size_t key_len, value_len;
    long long times_poisoned;
    char *key, *value; 
    int ret;
    int i;

   if (argc != 4)
        return RedisModule_WrongArity(ctx);

    key = RedisModule_StringPtrLen(argv[1], &key_len);
    if (RedisModule_StringToLongLong(argv[2], &value_len) == REDISMODULE_ERR)
	return REDISMODULE_ERR;
    if (RedisModule_StringToLongLong(argv[3], &times_poisoned) == REDISMODULE_ERR)
	return REDISMODULE_ERR;


    for (i = 0; i < times_poisoned; i++) {	
	value = malloc(value_len);
	if (!value) {      
	    RedisModule_ReplyWithSimpleString(ctx, "ERROR");
	    return REDISMODULE_OK;
	}
	    
	memset(value, 0xfe, value_len);
	ret = tmem_ioctl_put(key, key_len, value, value_len);
	if (ret)
	    break;

	free(value);
    }
    
    RedisModule_ReplyWithSimpleString(ctx, "OK");

    return REDISMODULE_OK;
}
