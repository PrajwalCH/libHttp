#include <stdio.h>
#include "request.h"

static void test_request_parser(void)
{
    char raw_request[] = "GET /test/api HTTP/1.1\r\nHost: http://localhost:8000\r\nAccept: text/html\r\nConnection: Alive\r\n";

    printf("---TEST REQUEST PARSER---\n");
    printf("Input: \n");
    printf("%s\n", raw_request);
    Request req = request_parse(raw_request);
    
    printf("Output: \n");
    printf("Method -> %s\n", req.method);
    printf("Uri -> %s\n", req.uri);
    printf("Protocol -> %s\n", req.protocol);

    printf("\n");

    printf("Headers size: %u\n", req.headers_size);
    for (size_t i = 0; i < req.headers_size; i++) {
        printf("%s -> %s\n", req.headers[i]->name, req.headers[i]->value);
    }
    request_dealloc(&req);
}

int main(int argc, char **argv)
{
    test_request_parser();
    return 0;
}

