#include "test.h"

void test1(hashmap *h)
{
    printf("\033[33mTEST 1\033[0m\n");

    hashmap_add(h, "num1", ANY(1));
    hashmap_add(h, "num2", ANY(2));
    hashmap_add(h, "num3", ANY(3));

    print_num_from_table(h, "num1");
    print_num_from_table(h, "num2");
    print_num_from_table(h, "num3");
    print_num_from_table(h, "num4");

    hashmap_set(h, "num2", ANY(8));
    print_num_from_table(h, "num2");

    printf("collisions: %lu\n\n", h->collisions);
}

void test2(hashmap *h, const char *file)
{
    printf("\033[33mTEST 2\033[0m\n");
    FILE *f = fopen(file, "w");

    char key[16];
    for (int i = 0; i < __INT16_MAX__; i++)
    {
        snprintf(key, sizeof(key), "num%lu", i);
        hashmap_set(h, key, ANY(i));
    }

    fclose(f);
    printf("written to '%s'\ncollisions: %lu\n\n", file, h->collisions);
    hashmap_print(h, f, print_int);
}

int main(int argc, char **argv)
{
    hashmap *h = hashmap_create(__INT16_MAX__, hashmap_default_hash, NULL);
    test1(h);
    hashmap_destroy(h);

    h = hashmap_create(__INT16_MAX__, hashmap_default_hash, NULL);
    test2(h, "test2.yaml");
    hashmap_destroy(h);

    return 0;
}
