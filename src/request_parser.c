#include "request.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "header_vector.h"
#include "state.h"
#include "tokenizer.h"

Request request_parse(const char *raw_request)
{
    Tokenizer tokenizer = tokenizer_init(raw_request);
    State state = STATE_METHOD;

    Request request;
    Header current_header;
    Header **headers = header_vector_alloc(5);

    while (tokenizer.data_idx < tokenizer.data_size) {
        switch (state) {
        case STATE_METHOD:
            if (tokenizer_peek(&tokenizer, 0) == ' ') {
                tokenizer_advance(&tokenizer);
                tokenizer_commit_and_advance_state(&tokenizer, &request.method, &state, STATE_URI);
                break;
            }
            tokenizer_consume_and_advance(&tokenizer);
            break;
        case STATE_URI:
            if (tokenizer_peek(&tokenizer, 0) == ' ') {
                tokenizer_advance(&tokenizer);
                tokenizer_commit_and_advance_state(&tokenizer, &request.uri, &state, STATE_PROTOCOL);
                break;
            }
            tokenizer_consume_and_advance(&tokenizer);
            break;
        case STATE_PROTOCOL:
            if (tokenizer_peek(&tokenizer, 0) == '\r' && tokenizer_peek(&tokenizer, 1) == '\n') {
                tokenizer_advance(&tokenizer);
                tokenizer_advance(&tokenizer);
                tokenizer_commit_and_advance_state(&tokenizer, &request.protocol, &state, STATE_HEADER_NAME);
                break;
            }
            tokenizer_consume_and_advance(&tokenizer);
            break;
        case STATE_HEADER_NAME:
            if (tokenizer_peek(&tokenizer, 0) == ':' && tokenizer_peek(&tokenizer, 1) == ' ') {
                tokenizer_advance(&tokenizer);
                tokenizer_advance(&tokenizer);
                tokenizer_commit_and_advance_state(&tokenizer, &current_header.name, &state, STATE_HEADER_VALUE);
                break;
            }
            tokenizer_consume_and_advance(&tokenizer);
            break;
        case STATE_HEADER_VALUE:
            if (tokenizer_peek(&tokenizer, 0) == '\r' && tokenizer_peek(&tokenizer, 1) == '\n') {
                tokenizer_advance(&tokenizer);
                tokenizer_advance(&tokenizer);
                tokenizer_commit_and_advance_state(&tokenizer, &current_header.value, &state, STATE_HEADER_NAME);
                header_vector_push(&headers, current_header);
                break;
            }
            tokenizer_consume_and_advance(&tokenizer);
            break;
        default:
            break;
        }
    }

    request.headers = headers;
    request.headers_size = header_vector_size();
    return request;
}
