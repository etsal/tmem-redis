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


/* Like TmemPut, but return the value to the client */
int TmemEcho(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
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

    RedisModule_ReplyWithSimpleString(ctx, value);
    return REDISMODULE_OK;
}



/* Like TmemDrop, but do not even reference the request */
int TmemIgnore(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
    if (argc != 3)
        return RedisModule_WrongArity(ctx);


    RedisModule_ReplyWithSimpleString(ctx, "OK");

    return REDISMODULE_OK;
}


/* Like TmemPut, but just drop the request */
int TmemDrop(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
    if (argc != 3)
        return RedisModule_WrongArity(ctx);

    size_t key_len, value_len;
   

    char *key = RedisModule_StringPtrLen(argv[1], &key_len);
    char *value = RedisModule_StringPtrLen(argv[2], &value_len); 

    RedisModule_ReplyWithSimpleString(ctx, "OK");

    return REDISMODULE_OK;
}


/* Like TmemPut, but accepts no input - it poisons the entry by filling it with a specific byte */
int TmemPoison(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    
   if (argc != 4)
        return RedisModule_WrongArity(ctx);

    size_t key_len, value_len;
    long long input_len, times_poisoned;
    char *value1, *value2; 
    char *key = RedisModule_StringPtrLen(argv[1], &key_len);
    int i;

    if (RedisModule_StringToLongLong(argv[2], &input_len) == REDISMODULE_ERR)
	return REDISMODULE_ERR;
    if (RedisModule_StringToLongLong(argv[3], &times_poisoned) == REDISMODULE_ERR)
	return REDISMODULE_ERR;
    value_len = (size_t) input_len;


    for (i = 0; i < times_poisoned; i++) {
	    value1 = malloc(value_len);
	    if (!value1) {
		fprintf(stderr, "malloc() failed\n");
		return REDISMODULE_OK;
	    }

	    value2 = malloc(value_len);
	    if (!value2) {
		free(value1);
		fprintf(stderr, "malloc() failed\n");
		return REDISMODULE_OK;
	    }
		
		
	    memcpy(value2, value1, value_len);

	    free(value1);
	    free(value2);
    }

    RedisModule_ReplyWithSimpleString(ctx, "Put OK");
    return REDISMODULE_OK;
}
