#pragma once

#include <stdio.h>

/* Parse arithmetic expression and evaluate it.
 * Return expression value and set is_correct to 1 if expression is correct.
 * Set is_correct to 0 if expression contains errors and print these errors to errors_file. */
int evaluate_arithmetic_expression(const char *string, int *is_correct, FILE *errors_file);
