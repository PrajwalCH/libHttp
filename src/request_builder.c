#include "request.h"

#include <stdbool.h>
#include <string.h>

#include "header_vector.h"
#include "sds.h"

static char *request_method_name(Method method)
{
    switch (method) {
    case METHOD_GET:
        return "GET";
        break;
    case METHOD_HEAD:
        return "HEAD";
        break;
    case METHOD_POST:
        return "POST";
        break;
    case METHOD_PUT:
        return "PUT";
        break;
    case METHOD_DELETE:
        return "DELETE";
        break;
    case METHOD_CONNECT:
        return "CONNECT";
        break;
    case METHOD_OPTIONS:
        return "OPTIONS";
        break;
    case METHOD_TRACE:
        return "TRACE";
        break;
    case METHOD_PATCH:
        return "PATCH";
        break;
    case METHOD_INVALID:
    default:
        return "INVALID";
        break;
    }
}

void request_set_method(Request *request, Method method)
{
    if (request->method == NULL)
        request->method = sdsnew(request_method_name(method));
}

void request_set_uri(Request *request, const char *uri)
{
    if (request->uri == NULL)
        request->uri = sdsnew(uri);
}

void request_set_protocol(Request *request, const char *protocol)
{
    if (request->protocol == NULL)
        request->protocol = sdsnew(protocol);
}

void request_set_header(Request *request, const char *name, const char *value)
{
    header_vector_push(&request->headers, (Header) {sdsnew(name), sdsnew(value)});
    request->headers_size++;
}

void request_set_headers(Request *request, Header **headers, size_t headers_size)
{
    for (size_t i = 0; i < headers_size; i++) {
        header_vector_push(&request->headers, *headers[i]);
    }
    request->headers_size = headers_size;
}

sds request_build(Request *request)
{
    // in case they are null
    request_set_method(request, METHOD_GET);
    request_set_uri(request, "/");
    request_set_protocol(request, "HTTP/1.1");

    sds raw_req = sdsempty();

    raw_req = sdscatsds(raw_req, request->method);
    raw_req = sdscat(raw_req, " ");
    raw_req = sdscatsds(raw_req, request->uri);
    raw_req = sdscat(raw_req, " ");
    raw_req = sdscatsds(raw_req, request->protocol);
    raw_req = sdscat(raw_req, "\r\n");

    if (request->headers_size == 0) {
        request_set_header(request, "Accept", "text/html,application/xhtml+xml");
        // FIXME: Add more important headers
    }

    for (size_t i = 0; i < request->headers_size; i++) {
        raw_req = sdscatsds(raw_req, request->headers[i]->name);
        raw_req = sdscat(raw_req, ": ");
        raw_req = sdscatsds(raw_req, request->headers[i]->value);
        raw_req = sdscat(raw_req, "\r\n");
    }
    raw_req = sdscat(raw_req, "\r\n");
    request_dealloc(request);
    return raw_req;
}

Request request_new(void)
{
    return (Request) {
        .method = NULL,
        .uri = NULL,
        .protocol = NULL,
        .headers = header_vector_alloc(5),
        .headers_size = 0
    };
}
