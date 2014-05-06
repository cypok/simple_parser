#include "lexer.h"

#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

struct _Lexer {
    const char *str;
    int number;
    Token token;
};

Lexer* lex_init(const char * string) {
    Lexer *lex = (Lexer*)malloc(sizeof(Lexer));
    if (lex == NULL) {
        return NULL;
    } else {
        lex->str = string;
        lex->number = 0;
        lex->token = NONE;
        return lex;
    }
}

void lex_destroy(Lexer *lex) {
    free(lex);
}

Token lex_next(Lexer *lex) {
    if (lex->token == EOS) {
        return EOS;
    }
    for (;;) {
        int c = *(lex->str++);

        if (c == '\0') {
            return lex->token = EOS;
        } else if (isspace(c)) {
            continue;
        } else if (isdigit(c)) {
            int num = c - '0';
            while (isdigit(*(lex->str))) {
                num = num * 10 + *(lex->str++) - '0';
            }
            lex->number = num;
            return lex->token = NUM;
        } else {
            return lex->token = c;
        }
    }
}

int lex_value(Lexer *lex) {
    assert(lex->token == NUM);
    return lex->number;
}

Token lex_lookahead(Lexer *lex) {
    return (lex->token == NONE) ? lex_next(lex) : lex->token;
}

