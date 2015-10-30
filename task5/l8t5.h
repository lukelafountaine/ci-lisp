#ifndef __l8t5_h_
#define __l8t5_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "y.tab.h"

int yyparse(void);
int yylex(void);
void yyerror(char *);

typedef enum { NUM_TYPE, FUNC_TYPE, LET_TYPE, LET_LIST, LET_ELEM, SYM} AST_NODE_TYPE;
typedef enum {NEG = 0, ABS, EXP, SQRT, EXP2, CBRT, ADD, SUB, MULT, DIV, MOD, LOG, POW, MAX, MIN, HYPOT} FUNC_NAMES;
extern char *arithmeticOps;

typedef struct
{
    double value;
} NUMBER_AST_NODE;

typedef struct
{
   char *name;
   struct ast_node *op1;
   struct ast_node *op2;
} FUNCTION_AST_NODE;

typedef struct
{
   struct ast_node *let_list;
   struct ast_node *s_expr;
} LET_AST_NODE;

typedef struct
{
   struct ast_node *let_elem;
   struct ast_node *let_list;
} LET_LIST_AST_NODE;

typedef struct
{
   char *symbol;
   //struct ast_node *value;
} LET_ELEM_AST_NODE;

typedef struct
{
   char *name;
} SYMBOL_AST_NODE;

typedef struct ast_node
{
   AST_NODE_TYPE type;
   union
   {
      NUMBER_AST_NODE number;
      FUNCTION_AST_NODE function;
      LET_AST_NODE let;
      LET_LIST_AST_NODE let_list;
      LET_ELEM_AST_NODE let_elem;
      SYMBOL_AST_NODE symbol;
   } data;
} AST_NODE;

AST_NODE *number(double value);
AST_NODE *function(char *funcName, AST_NODE *op1, AST_NODE *op2);
AST_NODE *let(AST_NODE *let_list, AST_NODE *s_expr);
AST_NODE *let_list(AST_NODE *let_elem, AST_NODE *let_list);
AST_NODE *let_elem(char *symbol, AST_NODE *s_expr);
AST_NODE *symbol(char *name);
void insertSymbol(char* name, AST_NODE* s_expr);
double getSymbolValue(char* name);

void freeNode(AST_NODE *p);

double eval(AST_NODE *ast);

#endif
