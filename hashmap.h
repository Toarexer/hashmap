#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>

#ifndef TABLE_KEY_MAX_LENGTH
#define TABLE_KEY_MAX_LENGTH 63
#endif
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

// Creates a new hashmap with the provided size and functions.
// The hash function is responsible for turning the string key into an index and the free function is resposible for freeing the stored value.
//
// The values are a (void *) and up to the user how they handle them.
//
// 'freefunc' can be NULL
hashmap *hashmap_create(size_t size, u_int64_t (*hashfunc)(char *, size_t), void (*freefunc)(void *));

// Frees the hashmap and it's elements. Frees the values if a freeing function was provided.
void hashmap_destroy(hashmap *hm);

// Gets the value from the key.
void *hashmap_get(hashmap *hm, char *key);

// Sets the value for a key.
// Adds the key if it is not part of the map yet.
void hashmap_set(hashmap *hm, char *key, void *value);

// Adds a new value to the hashmap.
// This does not check for exisitng keys and can create duplicates, unlike hasmap_set.
void hashmap_add(hashmap *hm, char *key, void *value);

// Removes a value from the hashmap.
//
// Returns 0 on success.
int hashmap_remove(hashmap *hm, char *key);

// Gets the hash (index) from the key.
//
// Very bad! Improve later!
u_int64_t hashmap_default_hash(char *key, size_t map_size);

// Print out the entire hashmap.
// The printfunc function takes in a hashmap_element pointer and fefines how the element should be printed.
// The default format for printing is ("\\t%s: 0x%08x\n", key, value).
//
// 'printfunc' can be NULL
void hashmap_print(hashmap *hm, FILE *f, void(printfunc)(FILE *f, hashmap_element *e));

#endif
