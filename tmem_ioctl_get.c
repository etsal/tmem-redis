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


/* Like Tmem Get, but return a random string of the requested length */
int TmemGenerate(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
 
    
    long long input_len; 
    size_t key_len, value_len;
    char *value ;

    if(RedisModule_StringToLongLong(argv[1], &input_len) == REDISMODULE_ERR)
	return REDISMODULE_ERR;

    value_len = (size_t) input_len;

    value = malloc(value_len);
    if (!value) {
        fprintf(stderr, "calloc() failed\n");
        return REDISMODULE_OK;
    }

    memset(value, 0x61, value_len);
	
    value[value_len - 1] = '\0';
    RedisModule_ReplyWithSimpleString(ctx, value);

    free(value);

    return REDISMODULE_OK;
    
}
    
/* Like TmemGet, but do not print out the result received */
int TmemSilent(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

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
	
	RedisModule_ReplyWithSimpleString(ctx, "OK");
    }

    free(value);

    return REDISMODULE_OK;
    
}


/* Like TmemSilentGet, but dirty all pages copied */
int TmemSilentDirty(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
 
    
    size_t key_len, value_len;
    char *key = RedisModule_StringPtrLen(argv[1], &key_len);    
    char *value ;
    int i;

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
	
	RedisModule_ReplyWithSimpleString(ctx, "OK");
    }

    for (i = 0; i < value_len / PAGE_SIZE; i += 1)
	value[i * PAGE_SIZE] = 0x61;

    free(value);

    return REDISMODULE_OK;
    
}
