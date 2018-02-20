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


/* Vanilla command. Write the output to the command line */
int TmemGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

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
        RedisModule_ReplyWithStringBuffer(ctx, value, value_len);
    
    free(value);

    return REDISMODULE_OK;
    
}


/* Vanilla command. Read the input from the command line */
int TmemPut(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
    int ret;
    size_t key_len, value_len;
    char *key, *value;

    if (argc != 3)
        return RedisModule_WrongArity(ctx);

    key = RedisModule_StringPtrLen(argv[1], &key_len);
    value = RedisModule_StringPtrLen(argv[2], &value_len);
    ret = tmem_ioctl_put(key, key_len, value, value_len, 0);

    if (ret)
        RedisModule_ReplyWithSimpleString(ctx, "ERROR");
    else 
	RedisModule_ReplyWithSimpleString(ctx, "OK");

    return REDISMODULE_OK;
}


/* Vanilla command. Invalidate a key */ 
int TmemInval(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
    int ret;
    size_t key_len;
    char *key;

    if (argc != 2)
        return RedisModule_WrongArity(ctx);

    key = RedisModule_StringPtrLen(argv[1], &key_len);
    ret = tmem_ioctl_inval(key, key_len, 0);

    if (ret)
        RedisModule_ReplyWithSimpleString(ctx, "ERROR");
    else 
	RedisModule_ReplyWithSimpleString(ctx, "OK");
        
    return REDISMODULE_OK;
}


/* Vanilla command. Write the output to the command line */
int ModuleGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    char *value = NULL;
    size_t value_len;

    RedisModuleKey *key;

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
  

    key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ);
    if (key) {
	value = RedisModule_StringDMA(key, &value_len, REDISMODULE_READ);
	RedisModule_ReplyWithStringBuffer(ctx, value, value_len);
	RedisModule_CloseKey(key);
    } else {
	RedisModule_ReplyWithSimpleString(ctx, "");
    }

    return REDISMODULE_OK;

}


/* Vanilla command. Write the output to the command line */
int ModuleSet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    RedisModuleKey *key;

    if (argc != 3)
        return RedisModule_WrongArity(ctx);

    key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ | REDISMODULE_WRITE);
    if (key) {
	RedisModule_StringSet(key, argv[2]);
	RedisModule_ReplyWithSimpleString(ctx, "OK");
	RedisModule_CloseKey(key);
    } else {
	RedisModule_ReplyWithSimpleString(ctx, "FAIL");
    }

    return REDISMODULE_OK;

}
