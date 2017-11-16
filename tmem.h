#ifndef _TMEM_H
#define _TMEM_H

#include <asm-generic/ioctl.h>

#define TMEM_MAGIC ('*') 

#define TMEM_GET (_IOW(TMEM_MAGIC, 1, long))
#define TMEM_PUT (_IOR(TMEM_MAGIC, 2, long))
#define TMEM_INVAL (_IO(TMEM_MAGIC, 3))
#define TMEM_CONTROL (_IO(TMEM_MAGIC, 4))

#define TMEM_PATH ("/dev/tmem_dev")
#define PAGE_SIZE (4096)
#define TMEM_MAX (1024 * 1024)


struct tmem_put_request {
    void *key;
    size_t key_len; 
    void *value;
    size_t value_len; 
};

struct tmem_get_request {
    void *key;
    size_t key_len; 
    void *value;
    size_t *value_lenp; 
};

struct tmem_invalidate_request {
    void *key;
    size_t key_len; 
};

struct tmem_answer{
	void *value;
	size_t *value_lenp;
};

struct tmem_request {
	union {
		struct tmem_put_request put;
		struct tmem_get_request get;
		struct tmem_invalidate_request inval;
	};
};



#endif /* _TMEM_H */
