#include "hashmap.h"

void print_num_from_table(hashmap *h, char *key)
{
    printf("%s = %ld\n", key, (long)hashmap_get(h, key));
}

void print_int(FILE *f, hashmap_element *e)
{
    fprintf(f, "\t%s: %d\n", e->key, e->value);
}

void print_float(FILE *f, hashmap_element *e)
{
    fprintf(f, "\t%s: %f\n", e->key, e->value);
}

void print_string(FILE *f, hashmap_element *e)
{
    fprintf(f, "\t%s: %s\n", e->key, e->value);
}

void print_test_struct(FILE *f, hashmap_element *e)
{
    fprintf(f, "\t%s: ?\n", e->key);
}
