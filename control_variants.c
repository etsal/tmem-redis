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

int TmemControl(RedisModuleCtx *ctx, int argc, unsigned long flags) {

    int ret;

    if (argc != 1)
        return RedisModule_WrongArity(ctx);
    
	
    ret = tmem_ioctl_control(flags);

    if (ret)
        RedisModule_ReplyWithSimpleString(ctx, "ERROR");
    else 
        RedisModule_ReplyWithSimpleString(ctx, "OK");

    return REDISMODULE_OK;
}


int TmemDummy(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    return TmemControl(ctx, argc, TCTRL_DUMMY);
}


int TmemReal(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    return TmemControl(ctx, argc, TCTRL_REAL);
}


int TmemSleepy(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    return TmemControl(ctx, argc, TCTRL_SLEEPY);
}


int TmemAwake(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    return TmemControl(ctx, argc, TCTRL_AWAKE);
}

int TmemSilent(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    return TmemControl(ctx, argc, TCTRL_SILENT);
}

int TmemAnswer(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    return TmemControl(ctx, argc, TCTRL_ANSWER);
}

int TmemGenerate(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    return TmemControl(ctx, argc, TCTRL_GENERATE);
}

int TmemInput(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    return TmemControl(ctx, argc, TCTRL_INPUT);
}

int TmemGenerateSize(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    int ret;
    unsigned long size;

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
    
    if (RedisModule_StringToLongLong(argv[1], &size) != REDISMODULE_OK) {
        RedisModule_ReplyWithSimpleString(ctx, "ERROR");
	return REDISMODULE_OK;	
    }
	
    ret = tmem_ioctl_generate_size(size);

    if (ret)
        RedisModule_ReplyWithSimpleString(ctx, "ERROR");
    else 
        RedisModule_ReplyWithSimpleString(ctx, "OK");

    return REDISMODULE_OK;
}
