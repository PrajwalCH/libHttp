#include "tokenizer.h"

#include <string.h>

Tokenizer tokenizer_init(const char *data)
{
    Tokenizer tokenizer = {
        .data = data,
        .data_idx = 0,
        .data_size = strlen(data),
        .current_token_idx = 0,
        .current_token = {0}
    };
    return tokenizer;
}

char tokenizer_peek(Tokenizer *tokenizer, size_t offset)
{
    if (tokenizer->data_idx + offset < tokenizer->data_size) {
        return tokenizer->data[tokenizer->data_idx + offset];
    } else {
        return '0';
    }
}

void tokenizer_advance(Tokenizer *tokenizer)
{
    if (tokenizer->data_idx < tokenizer->data_size) {
        tokenizer->data_idx++;
    }
}

void tokenizer_consume_and_advance(Tokenizer *tokenizer)
{
    if (tokenizer->current_token_idx < TOKEN_MAX_SIZE) {
        tokenizer->current_token[tokenizer->current_token_idx++] = tokenizer->data[tokenizer->data_idx++];
    }
}

static void tokenizer_clear_current_token(Tokenizer *tokenizer)
{
    memset(tokenizer->current_token, 0, tokenizer->current_token_idx);
    tokenizer->current_token_idx = 0;
}

void tokenizer_commit_and_advance_state(Tokenizer *tokenizer, sds *dest, State *old_state, State new_state)
{
    *dest = sdsnewlen(tokenizer->current_token, tokenizer->current_token_idx);
    tokenizer_clear_current_token(tokenizer);
    *old_state = new_state;
}
