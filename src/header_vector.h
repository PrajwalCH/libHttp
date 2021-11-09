#ifndef HEADER_VECTOR_H

#define HEADER_VECTOR_H

#include <stddef.h>

#include "header.h"

Header **header_vector_alloc(size_t capacity);
void header_vector_push(Header **headers, Header header);
void header_vector_free(Header **headers, size_t size);

#endif /* HEADER_VECTOR_H */

