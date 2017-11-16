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


#include "tmem.h"

extern int fd;

int tmem_get(void *key, size_t key_len, void *value, size_t *value_lenp) 
{

    int ret = 0;

    struct tmem_get_request tmem_get_request = {
        .key = key,
        .key_len = key_len,
	.value = value,
	.value_lenp = value_lenp,
    };

    if (ioctl(fd, TMEM_GET, &tmem_get_request)) {
	fprintf(stderr, "tmem_get: ioctl failed\n");
	ret = -1;
    }
    
    return ret;    
}

int tmem_put(void *key, size_t key_len, void *value, size_t value_len) 
{
    
    int ret = 0;

    struct tmem_put_request tmem_put_request = {
        .key = key,
        .key_len = key_len,
        .value = value,
        .value_len = value_len,
    };

    if (ioctl(fd, TMEM_PUT, &tmem_put_request)) {
	fprintf(stderr, "tmem_put: ioctl failed\n");
	ret = -1;
    }
    
    return ret;
}

int tmem_inval(void *key, size_t key_len) 
{
	
    int ret = 0;

    struct tmem_invalidate_request tmem_invalidate_request = {
        .key = key,
        .key_len = key_len,
    };

    if (ioctl(fd, TMEM_INVAL, &tmem_invalidate_request)) {
	fprintf(stderr, "tmem_inval: ioctl failed\n");
	ret = -1;
    }

    return ret;
}


