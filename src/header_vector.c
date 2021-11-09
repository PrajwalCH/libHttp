#include "header_vector.h"

#include <stdlib.h>

#define VECTOR_EXTEND_CAPACITY 5

static size_t vector_size = 0;
static size_t vector_capacity = 0;

static void header_vector_set_null(Header **headers, size_t upper_bound)
{
    if (num >= vector_capacity)
        return;

    for (size_t i = vector_size; i < upper_bound; i++) {
        headers[i] = NULL;
    }
}

static Header **header_vector_realloc(Header **headers, size_t capacity)
{
    headers = realloc(headers, capacity);
    if (headers == NULL)
        return NULL;
    vector_capacity += capacity;
    header_vector_set_null(headers, capacity);
    return headers;
}

void header_vector_push(Header **headers, Header header)
{
    if (vector_size >= vector_capacity) {
        headers = header_vector_realloc(headers, VECTOR_EXTEND_CAPACITY);
        vector_capacity += VECTOR_EXTEND_CAPACITY;
        header_vector_set_null(headers, VECTOR_EXTEND_CAPACITY);
    }
    headers[vector_size] = malloc(sizeof(Header));
    headers[vector_size]->name = header.name;
    headers[vector_size]->value = header.value;
    vector_size++;
}

void header_vector_dealloc(Header **headers, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        if (headers[i] == NULL)
            continue;
        free(headers[i]->name);
        free(headers[i]->value);
        free(headers[i]);
    }

    free(headers);
    headers = NULL;
}

Header **header_vector_alloc(size_t capacity)
{
    Header **headers = malloc(sizeof(Header *) * capacity);
    if (headers == NULL)
        return NULL;
    vector_capacity = capacity;
    header_vector_set_null(headers, capacity);
    return headers;
}
