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
#include "tmem_ioctl.h"

#define TMEM_PATH ("/dev/tmem_dev")
extern int fd;

/* Vanilla command. Write the output to the command line */
int TmemGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
 
    
    size_t key_len, value_len;
    char *key = RedisModule_StringPtrLen(argv[1], &key_len);    
    char *value ;

    value = malloc(TMEM_MAX);
    if (!value) {
        fprintf(stderr, "calloc() failed\n");
        return REDISMODULE_OK;
    }

    struct get_message get_message = {
        .key = key,
        .key_len = key_len,
	.value = value,
	.value_lenp = &value_len,
    };

    if (ioctl(fd, TMEM_GET, &get_message)) {
        RedisModule_ReplyWithSimpleString(ctx, "Get Failed (not empty, failed)"); 
    } else {
	
	value[value_len] = '\0';
        RedisModule_ReplyWithSimpleString(ctx, get_message.value);
    }

    free(value);

    return REDISMODULE_OK;
    
}

/* Vanilla command. Read the input from the command line */
int TmemPut(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
    if (argc != 3)
        return RedisModule_WrongArity(ctx);

    size_t key_len, value_len;
   

    char *key = RedisModule_StringPtrLen(argv[1], &key_len);
    char *value = RedisModule_StringPtrLen(argv[2], &value_len);

    struct put_message put_message = {
        .key = key,
        .key_len = key_len,
        .value = value,
        .value_len = value_len,
    };

    if (ioctl(fd, TMEM_PUT, &put_message)) {
        RedisModule_ReplyWithSimpleString(ctx, "Put failed");
        return REDISMODULE_ERR;
    }

    RedisModule_ReplyWithSimpleString(ctx, "Put OK");
    return REDISMODULE_OK;
}



/* Vanilla command. Invalidate a key */ 
int TmemInval(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
    
    size_t key_len;
    char *key = RedisModule_StringPtrLen(argv[1], &key_len);

    struct invalidate_message invalidate_message = {
        .key = key,
        .key_len = key_len,
    };

    if (ioctl(fd, TMEM_INVAL, &invalidate_message)) {
        RedisModule_ReplyWithSimpleString(ctx, "Invalidate failed");
        return REDISMODULE_OK;
    }

    RedisModule_ReplyWithSimpleString(ctx, "Invalidate OK");
    return REDISMODULE_OK;
}

int TmemDummy(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
        RedisModule_ReplyWithSimpleString(ctx, "Dummy done");
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
	RedisModule_ReplyWithSimpleString(ctx, value);

    } else {
	RedisModule_ReplyWithSimpleString(ctx, "");
    }

    if (key)
	RedisModule_CloseKey(key);

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

    } else {
	RedisModule_ReplyWithSimpleString(ctx, "FAIL");
    }

    if (key)
	RedisModule_CloseKey(key);

    return REDISMODULE_OK;

}
