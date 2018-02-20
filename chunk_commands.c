#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "redismodule.h"
#include "tmem.h"
#include "chunk.h"

#define KEY_TABLE ("KEY_TABLE")
#define CHUNK_TABLE ("CHUNK_TABLE")

/* TODO: Free Redis strings */

#define make_id(chunk_id, index) (((uint32_t) chunk_id) << 16 | index)
#define chunk_id(id) (id >> 16)
#define index(id) (id | 0xFFFF)


RedisModuleKey *openTable(RedisModuleCtx *ctx, char *name)
{
    RedisModuleString *table_name;

    table_name = RedisModule_CreateString(ctx, name, sizeof(name));
    return RedisModule_OpenKey(ctx, table_name, REDISMODULE_READ | REDISMODULE_WRITE); 
}

/* TODO: Maybe abort if not enough keys? */
int ChunkFill(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
{
    RedisModuleCallReply *reply;
    RedisModuleCallReply *keys;
    RedisModuleCallReply *reply_key;
    RedisModuleString *key_string;
    RedisModuleKey *key;
    RedisModuleString *numstr;
    RedisModuleKey *id_table;
    struct chunk *chunk;
    char member_count[10];
    size_t len;
    char *str;
    int i;
    uint32_t id;
    uint16_t index;

    if (argc != 1)
        return RedisModule_WrongArity(ctx);


    /* Chunk creation */
    chunk = createChunk();
    if (!chunk)
        goto out;


    /* Get as many keys as possible */
    sprintf(member_count, "%u\0", MAX_MEMBERS); 
    reply = RedisModule_Call(ctx, "SCAN", "ccc", "0", "COUNT", member_count);

    if (RedisModule_CallReplyType(reply) != REDISMODULE_REPLY_ARRAY)
        goto out;
 
    keys = RedisModule_CallReplyArrayElement(reply, 1);
    if (RedisModule_CallReplyType(keys) != REDISMODULE_REPLY_ARRAY)
        goto out;



    /* Register chunk */
    numstr = RedisModule_CreateStringFromLongLong(ctx, chunk->id);

    id_table = openTable(ctx, CHUNK_TABLE);
    RedisModule_HashSet(id_table, REDISMODULE_HASH_NONE, numstr, numstr, NULL);
    RedisModule_CloseKey(id_table);



    /* Open hashtables */
    id_table = openTable(ctx, KEY_TABLE);

    /* 
     *  Iterate through the keys, sending them to the chunk, 
     *  as well as deleting them from Redis.
     */
    for (i = 0; (reply_key = RedisModule_CallReplyArrayElement(keys, i)) !=  NULL; i++) { 

        key_string = RedisModule_CreateStringFromCallReply(reply_key);
        key = RedisModule_OpenKey(ctx, key_string, REDISMODULE_READ | REDISMODULE_WRITE);
        assert(key);

        /* Only strings supported for now */
        if (RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_STRING) {
            RedisModule_CloseKey(key);
            continue;
        }
        
        /* The actual addition to the chunk */
        str = RedisModule_StringDMA(key, &len, REDISMODULE_WRITE);
        index = addPair(chunk, str, len);
        if (index == ADDPAIR_ERROR) {
            RedisModule_CloseKey(key);
            break;
        }

        RedisModule_DeleteKey(key);
        RedisModule_CloseKey(key);


        /* Update the index table */
        id = make_id(chunk->id, index);

        numstr = RedisModule_CreateStringFromLongLong(ctx, id);
        RedisModule_HashSet(id_table, REDISMODULE_HASH_CFIELDS, str, numstr, NULL);
    }
    
    RedisModule_CloseKey(id_table);

    
    RedisModule_ReplyWithLongLong(ctx, chunk->id);
    return REDISMODULE_OK;

out:

    freeChunk(chunk); 
    RedisModule_ReplyWithSimpleString(ctx, "Something went wrong");

    return REDISMODULE_OK; 
}


int ChunkKey(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
{
    RedisModuleKey *id_table;
    RedisModuleString *result;

    if (argc != 2)
        return RedisModule_WrongArity(ctx);

    id_table = openTable(ctx, KEY_TABLE);
    RedisModule_HashGet(id_table, REDISMODULE_HASH_NONE, argv[1], &result, NULL); 
    RedisModule_CloseKey(id_table);

    if (result)
        RedisModule_ReplyWithString(ctx, result);
    else
        RedisModule_ReplyWithSimpleString(ctx, "");

    return REDISMODULE_OK; 
}


int ChunkInMemory(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
{
    RedisModuleKey *id_table;
    RedisModuleString *result;

    if (argc != 2)
        return RedisModule_WrongArity(ctx);

    id_table = openTable(ctx, CHUNK_TABLE);
    RedisModule_HashGet(id_table, REDISMODULE_HASH_NONE, argv[1], &result, NULL); 
    RedisModule_CloseKey(id_table);

    if (result)
        RedisModule_ReplyWithSimpleString(ctx, "Yes");
    else
        RedisModule_ReplyWithSimpleString(ctx, "No");

    return REDISMODULE_OK; 
}


