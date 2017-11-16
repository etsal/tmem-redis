#ifndef _TMEM_IOCTL_H
#define _TMEM_IOCTL_H


int tmem_ioctl_get(void *key, size_t key_len, void *value, size_t *value_lenp);
int tmem_ioctl_put(void *key, size_t key_len, void *value, size_t value_len);
int tmem_ioctl_inval(void *key, size_t key_len); 
int tmem_ioctl_control(long flags); 

int fd;

#endif /* _TMEM_IOCTL_H */
