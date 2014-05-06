#include "lexer.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/*
E -> T { [+−] T }*
T -> F { [* /] F }*
F -> NUM | ( E )
*/

typedef struct _Context {
    Lexer *lex;
    int *correct;
    FILE *errors_file;
} Context;

static char * token2str(Token token) {
    switch (token) {
        case EOS: return "end of string";
        case NONE: return "none";

        case NUM: return "number";

        default: return NULL;
    }
}

static void error(Context *ctx, const char *fmt, ...) {
	va_list arglist;
	va_start(arglist, fmt);

    fprintf(ctx->errors_file, "Error: ");
	vfprintf(ctx->errors_file, fmt, arglist);
    fprintf(ctx->errors_file, "\n");

	va_end(arglist);

    *ctx->correct = 0;
}

static void match(Context *ctx, int exp_token) {
    if (!*ctx->correct) return;

    int act_token = lex_lookahead(ctx->lex);

    if (act_token != exp_token) {
        char exp_buf[] = {exp_token, '\0'};
        char *exp_str = token2str(exp_token);
        if (exp_str == NULL) {
            exp_str = exp_buf;
        }

        char act_buf[] = {act_token, '\0'};
        char *act_str = token2str(act_token);
        if (act_str == NULL) {
            act_str = act_buf;
        }

        error(ctx, "expected '%s', but got '%s'", exp_str, act_str);
    }
}

static void match_and_next(Context *ctx, int token) {
    match(ctx, token);
    lex_next(ctx->lex);
}

static int E(Context *ctx);

static int F(Context *ctx) {
    if (lex_lookahead(ctx->lex) == LP) {
        lex_next(ctx->lex);
        int value = E(ctx);
        match_and_next(ctx, RP);
        return value;
    } else {
        match(ctx, NUM);
        if (!*ctx->correct) return 0;
        int value = lex_value(ctx->lex);
        lex_next(ctx->lex);
        return value;
    }
}

static int T(Context *ctx) {
    int value = F(ctx);
    if (!*ctx->correct) return 0;
    for (;;) {
        if (lex_lookahead(ctx->lex) == MUL) {
            lex_next(ctx->lex);
            value *= F(ctx);
            if (!*ctx->correct) return 0;
        } else if (lex_lookahead(ctx->lex) == DIV) {
            lex_next(ctx->lex);
            value /= F(ctx);
            if (!*ctx->correct) return 0;
        } else {
            break;
        }
    }
    return value;
}

static int E(Context *ctx) {
    int value = T(ctx);
    if (!*ctx->correct) return 0;
    for (;;) {
        if (lex_lookahead(ctx->lex) == ADD) {
            lex_next(ctx->lex);
            value += T(ctx);
            if (!*ctx->correct) return 0;
        } else if (lex_lookahead(ctx->lex) == SUB) {
            lex_next(ctx->lex);
            value -= T(ctx);
            if (!*ctx->correct) return 0;
        } else {
            break;
        }
    }
    return value;
}

int evaluate_arithmetic_expression(const char *string, int *is_correct, FILE *errors_file) {
    Context ctx;
    ctx.errors_file = errors_file;
    ctx.correct = is_correct;
    *ctx.correct = 1;
    ctx.lex = lex_init(string);
    if (ctx.lex == NULL) {
        error(&ctx, "failed to create lexer");
        return 0;
    }

    int value = E(&ctx);
    if (*ctx.correct) {
        match(&ctx, EOS);
    }

    lex_destroy(ctx.lex);
    return *ctx.correct ? value : 0;
}
