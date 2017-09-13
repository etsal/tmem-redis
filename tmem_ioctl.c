#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include "redismodule.h"
#include "tmem_ioctl.h"

#define TMEM_PATH ("/dev/tmem_dev")
extern int fd;

int TmemGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
 
    
    size_t key_len, value_len;
    const char *key = RedisModule_StringPtrLen(argv[1], &key_len);
    char *value;

    value = calloc(PAGE_SIZE, sizeof(char));
    if (!value) {
        fprintf(stderr, "calloc() failed\n");
        return REDISMODULE_OK;
    }

    struct key_value pair = {
        .key = key,
        .key_len = key_len,
        .value = value,
        .value_len = value_len,
    };

    if (ioctl(fd, TMEM_GET, (unsigned long) &pair)) {
        RedisModule_ReplyWithSimpleString(ctx, "No such key");
    } else {
        RedisModule_ReplyWithSimpleString(ctx, pair.value);
    }

    free(value);
    return REDISMODULE_OK;
    
}

int TmemPut(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
    if (argc != 3)
        return RedisModule_WrongArity(ctx);

    size_t key_len, value_len;
   

    const char *key = RedisModule_StringPtrLen(argv[1], &key_len);
    const char *value = RedisModule_StringPtrLen(argv[2], &value_len);

    struct key_value pair = {
        .key = key,
        .key_len = key_len,
        .value = value,
        .value_len = value_len,
    };

    if (ioctl(fd, TMEM_PUT, (unsigned long) &pair)) {
        RedisModule_ReplyWithSimpleString(ctx, "Put failed");
        return REDISMODULE_ERR;
    }

    RedisModule_ReplyWithSimpleString(ctx, "Put OK");
    return REDISMODULE_OK;
}

int TmemInval(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
    
    size_t key_len;
    const char *key = RedisModule_StringPtrLen(argv[1], &key_len);

    struct key_value pair = {
        .key = key,
        .key_len = key_len,
    };

    if (ioctl(fd, TMEM_INVAL, (unsigned long) &pair)) {
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
