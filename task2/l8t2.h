#ifndef __l8t3_h_
#define __l8t3_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "y.tab.h"

int yyparse(void);
int yylex(void);
void yyerror(char *);

typedef enum {NEG = 0, ABS, EXP, SQRT, ADD, SUB, MULT, DIV, MOD, LOG, POW, MAX, MIN, EXP2, CBRT, HYPOT} FUNC_NAMES;

// function to calculate expressions
double calc(char *, double, double);

#endif
