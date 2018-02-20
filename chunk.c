#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redismodule.h"
#include "tmem.h"
#include "chunk.h"


static uint16_t cur_id = 0;

struct chunk *createChunk() 
{
    struct chunk *chunk = malloc(sizeof(struct chunk));
    if (!chunk)
        return NULL;

    chunk->id = cur_id++;
    chunk->cur_size=  0;
    chunk->indices[0] = 0;
    chunk->cur_index = 0;

    return chunk;
    
}

void freeChunk(struct chunk *chunk) 
{
    free(chunk);
}

uint16_t addPair(struct chunk *chunk, void *data, size_t len) 
{
    uint16_t index = chunk->indices[chunk->cur_index];
    uint16_t predicted_size = chunk->cur_size; 


    predicted_size  += len + sizeof(size_t);
    if (predicted_size >= CHUNK_SIZE || chunk->cur_index >= MAX_MEMBERS)
        return ADDPAIR_ERROR;

    memcpy((&chunk->contents[index]), data, len); 
    index += len;

    memcpy((&chunk->contents[index]), &len, sizeof(size_t)); 
    index += sizeof(size_t);

    chunk->cur_index += 1;
    chunk->indices[chunk->cur_index] = index;
    chunk->cur_size = predicted_size;
    
    return chunk->cur_index - 1;
}





