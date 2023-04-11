#include <string.h>
#include "hashmap.h"

hashmap_element *create_element(char *key, void *value, hashmap_element *prev)
{
    hashmap_element *e = (hashmap_element *)malloc(sizeof(hashmap_element));
    size_t keylen = strnlen(key, 0xff);

    e->value = value;
    e->key = (char *)malloc(keylen);
    e->prev = prev;
    e->next = NULL;

    memcpy(e->key, key, keylen);
    return e;
}

void free_element(hashmap_element *e)
{
    free(e->key);
    free(e);
}

void free_element_and_value(hashmap_element *e, void (*freefunc)(void *))
{
    if (e->value != NULL)
        freefunc(e->value);
    free(e->key);
    free(e);
}

hashmap_element *get_element(hashmap *hm, char *key, u_int64_t *index)
{
    u_int64_t i = hm->hashfunc(key, hm->size);
    if (index != NULL)
        *index = i;
    hashmap_element *e = hm->data[i];
    while (e != NULL)
    {
        if (strcmp(e->key, key) == 0)
            return e;
        e = e->next;
    }
    return NULL;
}

hashmap *hashmap_create(size_t size, u_int64_t (*hashfunc)(char *, size_t), void (*freefunc)(void *))
{
    hashmap *hm = (hashmap *)malloc(sizeof(hashmap));
    hm->size = size;
    hm->count = 0;
    hm->collisions = 0;
    hm->data = (hashmap_element **)calloc(sizeof(hashmap_element *), hm->size);
    hm->hashfunc = hashfunc;
    hm->freefunc = freefunc;
    return hm;
}

void hashmap_destroy(hashmap *hm)
{
    if (hm->freefunc != NULL)
    {
        for (size_t i = 0; i < hm->size; i++)
        {
            hashmap_element *current = hm->data[i], *next;
            while (current != NULL)
            {
                next = current->next;
                free_element_and_value(current, hm->freefunc);
                current = next;
            }
        }
    }
    else
    {
        for (size_t i = 0; i < hm->size; i++)
        {
            hashmap_element *current = hm->data[i], *next;
            while (current != NULL)
            {
                next = current->next;
                free_element(current);
                current = next;
            }
        }
    }
    free(hm->data);
    free(hm);
}

void *hashmap_get(hashmap *hm, char *key)
{
    hashmap_element *e = get_element(hm, key, NULL);
    return e != NULL ? e->value : NULL;
}

void hashmap_set(hashmap *hm, char *key, void *value)
{
    hashmap_element *e = get_element(hm, key, NULL);
    if (e == NULL)
        hashmap_add(hm, key, value);
    else
        e->value = value;
}

void hashmap_add(hashmap *hm, char *key, void *value)
{
    u_int64_t index = hm->hashfunc(key, hm->size);
    hashmap_element *e = hm->data[index];

    if (e == NULL)
    {
        hm->data[index] = create_element(key, value, e);
        return;
    }
    while (e->next != NULL)
        e = e->next;
    e->next = create_element(key, value, e);
}

int hashmap_remove(hashmap *hm, char *key)
{
    u_int64_t index;
    hashmap_element *e = get_element(hm, key, &index);
    if (e != NULL)
    {
        if (e->prev == NULL)
            hm->data[index] = e->next;
        else
            e->prev->next = e->next;
        e->next->prev = e->prev;

        if (hm->freefunc != NULL)
            free_element_and_value(e, hm->freefunc);
        else
            free_element(e);
        return 0;
    }
    return 1;
}

u_int64_t hashmap_default_hash(char *key, size_t map_size)
{
    u_int64_t val = 0;
    for (int i = 0, x = 0; key[i] != '\0' && i < TABLE_KEY_MAX_LENGTH; i++)
    {
        val += key[i] << x;
        x = (x + 1) & 31; // ((sizeof(val) * 8) - 1)
    }
    return val % map_size;
}
