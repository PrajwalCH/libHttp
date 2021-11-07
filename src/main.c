#include <stdio.h>
#include "request.h"

int main(int argc, char **argv)
{
    char *req_msg = "GET /id/1200?name=prajwal HTTP/1.1\r\nContent-Length: 12\r\n";
    Request req = request_parse(argv[1] ? argv[1] : req_msg);

    printf("method: %s\nuri: %s\nprotocol: %s\n", req.method, req.uri, req.protocol);
    
    printf("\n---Headers---\n");
    printf("headers size: %u\n", req.headers_size);
    for (size_t i = 0; i < req.headers_size; i++) {
        printf("%s -> %s\n", req.headers[i]->name, req.headers[i]->value);
    }

    request_dealloc_parsed(&req);
    return 0;
}

