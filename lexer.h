#pragma once

typedef enum _Token {
    LP = '(', RP = ')',
    ADD = '+', SUB = '-',
    MUL = '*', DIV = '/',
    NUM = 256,
    EOS,
    NONE
} Token;

typedef struct _Lexer Lexer;

/* New lexer from string. */
Lexer* lex_init(const char * string);

/* Remove current and get next. */
Token lex_next(Lexer *lex);

/* Get current. */
Token lex_lookahead(Lexer *lex);

/* Attribute of current. */
int lex_value(Lexer *lex);

void lex_destroy(Lexer *lex);
