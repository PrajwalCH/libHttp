#ifndef TOKENIZER_H

#define TOKENIZER_H

#include <stddef.h>

#include "sds.h"
#include "state.h"

#define TOKEN_MAX_SIZE 255

typedef struct Tokenizer {
    const char *data;
    size_t data_idx;
    size_t data_size;
    size_t current_token_idx;
    char   current_token[TOKEN_MAX_SIZE + 1];
} Tokenizer;

Tokenizer tokenizer_init(const char *data);
char tokenizer_peek(Tokenizer *tokenizer, size_t offset);
void tokenizer_advance(Tokenizer *tokenizer);
void tokenizer_consume_and_advance(Tokenizer *tokenizer);
void tokenizer_commit_and_advance_state(Tokenizer *tokenizer, sds *dest, State *old_state, State new_state);

#endif /* TOKENIZER_H */

