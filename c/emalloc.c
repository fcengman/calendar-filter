#include <stdlib.h>
#include <stdio.h>
#include "emalloc.h"

void *emalloc(size_t n)
{
    void *p;

    p = malloc(n);
    if (p == NULL)
    {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}

void *ecalloc(size_t n, size_t size)
{
    void *p;

    p = calloc(n, size);
    if (p == NULL)
    {
        fprintf(stderr, "calloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}
