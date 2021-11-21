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
    if (tokenizer->data_idx + offset >= tokenizer->data_size) return '0';
    return tokenizer->data[tokenizer->data_idx + offset];
}

void tokenizer_advance(Tokenizer *tokenizer)
{
    if (tokenizer->data_idx >= tokenizer->data_size) return;
    tokenizer->data_idx++;
}

void tokenizer_consume_and_advance(Tokenizer *tokenizer)
{
    if (tokenizer->current_token_idx >= TOKEN_MAX_SIZE) return;
    tokenizer->current_token[tokenizer->current_token_idx++] = tokenizer->data[tokenizer->data_idx++];
}

static void tokenizer_clear_current_token(Tokenizer *tokenizer)
{
    memset(tokenizer->current_token, 0, tokenizer->current_token_idx);
    tokenizer->current_token_idx = 0;
}

void tokenizer_commit(Tokenizer *tokenizer, sds *dest)
{
    *dest = sdsnewlen(tokenizer->current_token, tokenizer->current_token_idx);
    tokenizer_clear_current_token(tokenizer);
}
