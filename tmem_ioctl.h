#ifndef _TMEMIOCTL_H
#define _TMEMIOCTL_H

#define TMEM_GET (1074276865) 
#define TMEM_PUT (2148018690) 
#define TMEM_INVAL (10755)

#define PAGE_SIZE (4096)

struct key_value {
    const void *key;
    size_t key_len; 
    const void *value;
    size_t value_len; 
};


#endif /* _TMEMIOCTL_H */
