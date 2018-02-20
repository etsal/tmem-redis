.PHONY: all clean

FLAGS=-Wall -Wno-discarded-qualifiers -Wno-unused-but-set-variable -Wno-unused-variable

all: tmem.so

tmem.so: chunk.o chunk_commands.o base_ops.o put_variants.o get_variants.o control_variants.o tmem.o loader.o
	gcc $^ -fPIC -shared -g -o tmem.so
    
%.o: %.c
	gcc $(FLAGS) -fPIC -g -c $^ -o $@

clean:
	rm -rf *.so *.o
