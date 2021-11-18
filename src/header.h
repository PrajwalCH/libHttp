#ifndef HEADER_H

#define HEADER_H

#include "sds.h"

typedef struct Header {
    sds name;
    sds value;
} Header;

#endif /* HEADER_H */

