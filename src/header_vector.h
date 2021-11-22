#ifndef HEADER_VECTOR_H

#define HEADER_VECTOR_H

#include <stddef.h>

#include "header.h"

Header **header_vector_alloc(size_t capacity);
void header_vector_dealloc(Header **headers, size_t size);
void header_vector_push(Header ***headers, Header header);
size_t header_vector_size(void);

#endif /* HEADER_VECTOR_H */

