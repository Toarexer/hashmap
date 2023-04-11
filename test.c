#include <stdio.h>
#include "hashmap.h"

void printnum(hashmap *h, char *key)
{
    printf("%s = %ld\n", key, (long)hashmap_get(h, key));
}

int main(int argc, char **argv)
{
    hashmap *h = hashmap_create(__INT16_MAX__, hashmap_default_hash, NULL);

    hashmap_add(h, "num1", ANY(1));
    hashmap_add(h, "num2", ANY(2));
    hashmap_add(h, "num3", ANY(3));

    printnum(h, "num1");
    printnum(h, "num2");
    printnum(h, "num3");
    printnum(h, "num4");

    hashmap_set(h, "num2", ANY(8));
    printnum(h, "num2");

    hashmap_destroy(h);
    return 0;
}