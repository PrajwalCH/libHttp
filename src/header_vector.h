#ifndef HEADER_VECTOR_H

#define HEADER_VECTOR_H

#include <stddef.h>

#include "header.h"

size_t header_vector_size(void);
void header_vector_push(Header ***headers, Header header);
void header_vector_dealloc(Header **headers, size_t size);
Header **header_vector_alloc(size_t capacity);

#endif /* HEADER_VECTOR_H */

