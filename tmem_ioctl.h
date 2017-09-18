#ifndef _TMEMIOCTL_H
#define _TMEMIOCTL_H

#define TMEM_GET (1074276865) 
#define TMEM_PUT (2148018690) 
#define TMEM_INVAL (10755)

#define PAGE_SIZE (4096)

struct put_message {
    void *key;
    size_t key_len; 
    void *value;
    size_t value_len; 
};

struct get_message {
    void *key;
    size_t key_len; 
    void *value;
    size_t *value_lenp; 
};

struct invalidate_message {
    void *key;
    size_t key_len; 
};

#endif /* _TMEMIOCTL_H */
