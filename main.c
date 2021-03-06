#include <stdio.h>

#include "lexer.h"
#include "parser.h"

void lexer_demo(char *input) {
    printf("Tokenization of <%s>: ", input);

    Lexer *lex = lex_init(input);
    Token token;
    do {
        token = lex_next(lex);
        switch (token) {
            case LP:  printf("LP ");  break;
            case RP:  printf("RP ");  break;

            case ADD: printf("ADD "); break;
            case SUB: printf("SUB "); break;
            case MUL: printf("MUL "); break;
            case DIV: printf("DIV "); break;

            case NUM: printf("%d ", lex_value(lex)); break;

            case EOS: printf("EOS "); break;
            case NONE: break;

            default: printf("%c ", token); break;
        }
    } while (token != EOS);
    printf("\n");
    lex_destroy(lex);
}

void parser_demo(char *input) {
    int correct;
    int value = evaluate_arithmetic_expression(input, &correct, stdout);
    if (correct) {
        printf("Evaluated %s to %d\n", input, value);
    } else {
        printf("There were errors while evaluation of <%s>.\n", input);
    }
}

int main(int argc, char *argv[]) {
    char *input;
    switch (argc) {
        case 1:
            input = "2 + 2 * 2 + (37 - 35)";
            break;

        case 2:
            input = argv[1];
            break;

        default:
            printf("Usage: %s [string]", argv[0]);
            return 1;
    }

    lexer_demo(input);

    parser_demo(input);

    return 0;
}
