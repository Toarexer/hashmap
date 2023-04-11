#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>

#define TABLE_KEY_MAX_LENGTH 128
#define ANY(v) ((void *)v)

typedef struct hashmap_element
{
    void *value;
    char *key;
    struct hashmap_element *prev;
    struct hashmap_element *next;
} hashmap_element;

typedef struct hashmap
{
    size_t size;
    size_t count;
    size_t collisions;
    hashmap_element **data;
    u_int64_t (*hashfunc)(char *, size_t);
    void (*freefunc)(void *);
} hashmap;

// Create a new hashmap
//
// 'freefunc' can be NULL
hashmap *hashmap_create(size_t size, u_int64_t (*hashfunc)(char *, size_t), void (*freefunc)(void *));

// Free the hashmap and it's children if a function was provided.
void hashmap_destroy(hashmap *hm);

// Get the value from the key.
void *hashmap_get(hashmap *hm, char *key);

// Set the value for a key. Add the key if it is not part of the map yet.
void hashmap_set(hashmap *hm, char *key, void *value);

// Add a new value to the hashmap.
void hashmap_add(hashmap *hm, char *key, void *value);

// Remove a value from the hashmap.
//
// Returns 0 on success.
int hashmap_remove(hashmap *hm, char *key);

// Get the hash (index) from the key.
u_int64_t hashmap_default_hash(char *key, size_t map_size);

#endif
