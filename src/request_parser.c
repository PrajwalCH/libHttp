#include "request.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_CAPACITY 256

#define PEEK(request) \
    peek(0, request)

#define PEEK_TO(n, request) \
    peek(n, request)

static size_t raw_request_size = 0;
static size_t index = 0;
static size_t buffer_size = 0;
static size_t headers_size = 0;
static size_t headers_capacity = 0;

typedef enum State {
    STATE_METHOD,
    STATE_URI,
    STATE_PROTOCOL,
    STATE_HEADER_NAME,
    STATE_HEADER_VALUE
} State;

static void headers_set_null(Header **headers, size_t num)
{
    if (num >= headers_capacity)
        return;

    for (size_t i = headers_size; i < num; i++) {
        headers[i] = NULL;
    }
}

static Header **headers_realloc(Header **headers, size_t capacity)
{
    headers = realloc(headers, capacity);
    if (headers == NULL)
        return NULL;
    headers_capacity += capacity;
    headers_set_null(headers, capacity);
    return headers;
}

static Header **headers_alloc(size_t capacity)
{
    Header **headers = malloc(sizeof(Header *) * capacity);
    if (headers == NULL)
        return NULL;
    headers_capacity = capacity;
    headers_set_null(headers, 5);
    return headers;
}

static void headers_free(Header **headers)
{
    for (size_t i = 0; i < headers_capacity; i++) {
        if (headers[i] == NULL)
            continue;
        free(headers[i]->name);
        free(headers[i]->value);
        free(headers[i]);
    }

    free(headers);
    headers = NULL;
}

static void headers_push(Header **headers, Header header)
{
    if (headers_size >= headers_capacity) {
        headers = realloc(headers, 5);
        headers_capacity += 5;
        headers_set_null(headers, 5);
    }
    headers[headers_size] = malloc(sizeof(Header));
    headers[headers_size]->name = header.name;
    headers[headers_size]->value = header.value;
    headers_size++;
}

static void buffer_clear(char *buffer)
{
    memset(buffer, 0, buffer_size);
    buffer_size = 0;
}

static void buffer_push(char *buffer, char ch)
{
    if (buffer_size >= BUFFER_CAPACITY)
        return;
    buffer[buffer_size++] = ch;
}

static void commit_and_advance_state(char *buffer, char **output, State *old_state, State new_state)
{
    *output = strndup(buffer, buffer_size);
    buffer_clear(buffer);
    *old_state = new_state;
}

static char consume(char *raw_request)
{
    if (index > raw_request_size)
        return 0;
    return raw_request[index++];
}

static char peek(int offset, char *raw_request)
{
    if (index + offset >= raw_request_size)
        return 0;
    return raw_request[index + offset];
}

Request request_parse(char *raw_request)
{
    raw_request_size = strlen(raw_request);
    State state = STATE_METHOD;

    char buffer[BUFFER_CAPACITY + 1] = {0};

    Request request = {
        .method = NULL,
        .uri = NULL,
        .protocol = NULL,
        .headers = NULL
    };
    Header current_header = {NULL, NULL};
    Header **headers = headers_alloc(5);

    while (index < raw_request_size) {
        switch (state) {
        case STATE_METHOD:
            if (PEEK(raw_request) == ' ') {
                consume(raw_request);
                commit_and_advance_state(buffer, &request.method, &state, STATE_URI);
                break;
            }
            buffer_push(buffer, consume(raw_request));
            break;
        case STATE_URI:
            if (PEEK(raw_request) == ' ') {
                consume(raw_request);
                commit_and_advance_state(buffer, &request.uri, &state, STATE_PROTOCOL);
                break;
            }
            buffer_push(buffer, consume(raw_request));
            break;
        case STATE_PROTOCOL:
            if (PEEK(raw_request) == '\r' && PEEK_TO(1, raw_request) == '\n') {
                consume(raw_request);
                consume(raw_request);
                commit_and_advance_state(buffer, &request.protocol, &state, STATE_HEADER_NAME);
                break;
            }
            buffer_push(buffer, consume(raw_request));
            break;
        case STATE_HEADER_NAME:
            if (PEEK(raw_request) == ':' && PEEK_TO(1, raw_request) == ' ') {
                consume(raw_request);
                consume(raw_request);
                commit_and_advance_state(buffer, &current_header.name, &state, STATE_HEADER_VALUE);
                break;
            }
            buffer_push(buffer, consume(raw_request));
            break;
        case STATE_HEADER_VALUE:
            if (PEEK(raw_request) == '\r' && PEEK_TO(1, raw_request) == '\n') {
                consume(raw_request);
                consume(raw_request);
                commit_and_advance_state(buffer, &current_header.value, &state, STATE_HEADER_NAME);
                headers_push(headers, current_header);
                break;
            }
            buffer_push(buffer, consume(raw_request));
            break;
        }
    }

    request.headers = headers;
    request.headers_size = headers_size;
    return request;
}

void request_dealloc_parsed(Request *request)
{
    free(request->method);
    free(request->uri);
    free(request->protocol);

    request->method = NULL;
    request->uri = NULL;
    request->protocol = NULL;

    headers_free(request->headers);
}

