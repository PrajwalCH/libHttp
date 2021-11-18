#ifndef REQUEST_H

#define REQUEST_H

#include <stddef.h>

#include "header.h"
#include "sds.h"

typedef enum Method {
    METHOD_GET,
    METHOD_HEAD,
    METHOD_POST,
    METHOD_PUT,
    METHOD_DELETE,
    METHOD_CONNECT,
    METHOD_OPTIONS,
    METHOD_TRACE,
    METHOD_PATCH,
    METHOD_INVALID
} Method;

typedef struct Request {
    sds method;
    sds uri;
    sds protocol;
    Header **headers;
    size_t headers_size;
} Request;

Request request_parse(const char *raw_request);
Request request_new(void);
void request_set_method(Request *request, Method method);
void request_set_uri(Request *request, const char *uri);
void request_set_protocol(Request *request, const char *protocol);
void request_set_header(Request *request, const char *name, const char *value);
void request_set_headers(Request *request, Header **headers, size_t headers_size);
sds request_build(Request *request);
void request_dealloc(Request *request);

#endif /* REQUEST_H */

