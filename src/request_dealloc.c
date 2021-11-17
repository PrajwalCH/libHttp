#include "request.h"

#include <stdio.h>
#include <stdlib.h>

#include "header_vector.h"

void request_dealloc(Request *request)
{
    free(request->method);
    free(request->uri);
    free(request->protocol);

    request->method = NULL;
    request->uri = NULL;
    request->protocol = NULL;

    header_vector_dealloc(request->headers, request->headers_size);
}
