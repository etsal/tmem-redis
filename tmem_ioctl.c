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

int TmemSilentGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
 
    
    size_t key_len, value_len;
    char *key = RedisModule_StringPtrLen(argv[1], &key_len);    
    char *value ;

    value = malloc(256 * PAGE_SIZE);
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
	RedisModule_ReplyWithSimpleString(ctx, "OK");
        //RedisModule_ReplyWithSimpleString(ctx, get_message.value);
    }

    free(value);

    return REDISMODULE_OK;
    
}

int TmemGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    if (argc != 2)
        return RedisModule_WrongArity(ctx);
 
    
    size_t key_len, value_len;
    char *key = RedisModule_StringPtrLen(argv[1], &key_len);    
    char *value ;

    value = malloc(256 * PAGE_SIZE);
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


int TmemFilePut(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    
    size_t key_len, value_len, unused;
    char *key = RedisModule_StringPtrLen(argv[1], &key_len);    
    char *value ;
    char *filename = RedisModule_StringPtrLen(argv[2], &unused);
    int ret = REDISMODULE_ERR;
    int put_fd;

    struct stat st;

    if (argc != 3)
        return RedisModule_WrongArity(ctx);

 
    put_fd = open(filename, O_RDONLY | O_CREAT);
    if (put_fd < 0)
	return REDISMODULE_ERR;

	
    if(fstat(put_fd, &st))
	goto fileput_out; 
    value_len = st.st_size;


    value = (char *) mmap(NULL, value_len, PROT_READ, MAP_SHARED, put_fd, 0); 
    if (value == MAP_FAILED)
	goto fileput_out; 


    struct put_message put_message = {
        .key = key,
        .key_len = key_len,
	.value = value,
	.value_len = value_len,
    };


    if (ioctl(fd, TMEM_PUT, &put_message)) 
        RedisModule_ReplyWithSimpleString(ctx, "Put failed"); 
    else 	
	RedisModule_ReplyWithSimpleString(ctx, "OK");
    


    if(munmap(value, value_len))
	goto fileput_out;


    ret = REDISMODULE_OK;

fileput_out:

    close(put_fd);
   
    return ret;
}


int TmemFileGet(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {

    
    size_t key_len, value_len, unused;
    char *key = RedisModule_StringPtrLen(argv[1], &key_len);    
    char *value ;
    char *filename = RedisModule_StringPtrLen(argv[2], &unused);
    int ret = REDISMODULE_ERR;
    int get_fd;

    if (argc != 3)
        return RedisModule_WrongArity(ctx);

 
    get_fd = open(filename, O_RDWR | O_CREAT);
    if (get_fd < 0)
	return REDISMODULE_ERR;

	
//    printf("Truncating...\n");
    if (ftruncate(get_fd, TMEM_MAX) < 0) 
	goto fileget_out; 

//    printf("Mapping...\n");

    value = (char *) mmap(NULL, TMEM_MAX, PROT_WRITE, MAP_SHARED, get_fd, 0); 
    if (value == MAP_FAILED) 
	goto fileget_out;
    
//    printf("Making message...\n");

    struct get_message get_message = {
        .key = key,
        .key_len = key_len,
	.value = value,
	.value_lenp = &value_len,
    };

//    printf("Ioctl...\n");

    if (ioctl(fd, TMEM_GET, &get_message))
        RedisModule_ReplyWithSimpleString(ctx, "File Get Failed (not empty, failed)"); 
    else 	
	RedisModule_ReplyWithSimpleString(ctx, "OK");
   
//    printf("Unmapping...\n");

    if(munmap(value, TMEM_MAX)) 
	goto fileget_out;
    
//    printf("Truncating again...\n");

    if (ftruncate(get_fd, value_len) < 0) 
	goto fileget_out;	

//    printf("Truncating successful\n");
	
    ret = REDISMODULE_OK;


fileget_out:

//    printf("Closing...\n");
    close(get_fd);

    if (ret == REDISMODULE_ERR)
	perror("TMEM.FILEGET");
   
//    printf("Done\n");
    return ret;
}
    

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
