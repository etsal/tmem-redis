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

int TmemControl(RedisModuleCtx *ctx, int argc, long flags) {

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


int Tmem(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    return TmemControl(ctx, argc, TCTRL_AWAKE);
}
