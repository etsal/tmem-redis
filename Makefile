.PHONY: all clean

all: tmem.so

tmem.so: chunk.o chunk_commands.o tmem_ioctl.o tmem_ioctl_get.o tmem_ioctl_put.o tmem_ioctl_file.o tmem.o
	gcc $^ -fPIC -shared -g -o tmem.so
    
%.o: %.c
	gcc -fPIC -g -c $^ -o $@

clean:
	rm -rf *.so *.o
