#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "request.h"

static void test_request_builder(void)
{
    printf("---TEST REQUEST BUILDER---\n");
    Request req = request_new();
    request_set_method(&req, METHOD_GET);
    request_set_uri(&req, "/test/api");
    request_set_protocol(&req, "HTTP/1.1");
    request_set_header(&req, "Host", "http://localhost:8000");

    printf("Input: \n");
    printf("Method -> %s\n", req.method);
    printf("Uri -> %s\n", req.uri);
    printf("Protocol -> %s\n", req.protocol);
    printf("Headers size: %u\n", req.headers_size);
    for (size_t i = 0; i < req.headers_size; i++) {
        printf("%s -> %s\n", req.headers[i]->name, req.headers[i]->value);
    }

    sds req_bytes = request_build(&req);
    printf("%s\n", req_bytes);
    sdsfree(req_bytes);
}

static void test_request_parser(void)
{
    char raw_request[] = "GET /test HTTP/1.1\r\nHost: http://localhost:8000\r\nAccept: text/html\r\nConnection: Alive\r\n\r\n";

    printf("---TEST REQUEST PARSER---\n");
    printf("Input: \n");
    printf("%s\n", raw_request);
    Request req = request_parse(raw_request);

    printf("Testing: \n");
    assert(strcmp(req.method, "GET") == 0);
    assert(strcmp(req.uri, "/test") == 0);
    assert(strcmp(req.protocol, "HTTP/1.1") == 0);
    assert(req.headers_size == 3);
    
    assert(strcmp(req.headers[0]->name, "Host") == 0);
    assert(strcmp(req.headers[0]->value, "http://localhost:8000") == 0);
    
    assert(strcmp(req.headers[1]->name, "Accept") == 0);
    assert(strcmp(req.headers[1]->value, "text/html") == 0);
    
    assert(strcmp(req.headers[2]->name, "Connection") == 0);
    assert(strcmp(req.headers[2]->value, "Alive") == 0);
    printf("Test passed\n");

    printf("---TEST REQUEST BUILDER---\n");
    sds req_bytes = request_build(&req);
    printf("Testing\n");
    assert(strcmp(raw_request, req_bytes) == 0);
    sdsfree(req_bytes);
}

int main(int argc, char **argv)
{
    test_request_parser();
    printf("\n");
    //test_request_builder();
    return 0;
}

