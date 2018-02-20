#ifndef _TMEM_H
#define _TMEM_H

#include <asm-generic/ioctl.h>

#define TMEM_MAGIC ('*') 

#define TMEM_GET (_IOR(TMEM_MAGIC, 1, struct tmem_request))
#define TMEM_PUT (_IOR(TMEM_MAGIC, 2, struct tmem_request))
#define TMEM_INVAL (_IOR(TMEM_MAGIC, 3, struct tmem_request))
#define TMEM_CONTROL (_IOR(TMEM_MAGIC, 4, long))
#define TMEM_GENERATE_SIZE (_IOR(TMEM_MAGIC, 5, long))

#define TMEM_PATH ("/dev/tmem_dev")
#define PAGE_SIZE (4096)
#define TMEM_MAX (1024 * 1024)

#define TCTRL_DUMMY (1UL << 0) 
#define TCTRL_REAL (1UL << 1) 
#define TCTRL_SLEEPY (1UL << 2) 
#define TCTRL_AWAKE (1UL << 3) 
#define TCTRL_SILENT (1UL << 4) 
#define TCTRL_ANSWER (1UL << 5) 
#define TCTRL_GENERATE (1UL << 6) 
#define TCTRL_INPUT (1UL << 7) 

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
	long flags;
};



#endif /* _TMEM_H */
