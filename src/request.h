#ifndef REQUEST_H

#define REQUEST_H

#include <stddef.h>

typedef struct Header {
    char *name;
    char *value;
} Header;

typedef struct Request {
    char *method;
    char *uri;
    char *protocol;
    Header **headers;
    size_t headers_size;
} Request;

Request request_parse(char *req_msg);
void request_dealloc_parsed(Request *req);

#endif /* REQUEST_H */

