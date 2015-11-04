#ifndef __l8t6_h_
#define __l8t6_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "y.tab.h"

int yyparse(void);
int yylex(void);
void yyerror(char *);

typedef enum { NUM_TYPE, FUNC_TYPE, LET_TYPE, SYM} AST_NODE_TYPE;
typedef enum {NEG = 0, ABS, EXP, SQRT, EXP2, CBRT, ADD, SUB, MULT, DIV, MOD, LOG, POW, MAX, MIN, HYPOT} FUNC_NAMES;

typedef struct scope_node
{
   struct symbol_ast_node* symbols;
   struct scope_node* parent;
} SCOPE_NODE;

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

typedef struct symbol_ast_node
{
   char* name;
   struct ast_node* value;
   struct symbol_ast_node* next;
} SYMBOL_AST_NODE;

typedef struct let_ast_node
{
   SCOPE_NODE* scope;
   struct ast_node* s_expr;
} LET_AST_NODE;

typedef struct ast_node
{
   AST_NODE_TYPE type;
   union
   {
      NUMBER_AST_NODE number;
      FUNCTION_AST_NODE function;
      LET_AST_NODE let;
      SYMBOL_AST_NODE symbol;
   } data;
} AST_NODE;

// functions for creating ast_nodes
AST_NODE* number(double value);
AST_NODE* function(char *funcName, AST_NODE *op1, AST_NODE *op2);
AST_NODE* let(SYMBOL_AST_NODE *symbols, AST_NODE *s_expr);
SYMBOL_AST_NODE* let_list(SYMBOL_AST_NODE *symbol, SYMBOL_AST_NODE *let_list);
SYMBOL_AST_NODE* let_elem(char *symbol, AST_NODE *s_expr);
AST_NODE* symbol(char *name);

// functions for doing stuff with the symbol table
double getSymbolValue(char* name);
void leaveScope();
void enterScope(SCOPE_NODE* newScope);

// functions for other stuff
double eval(AST_NODE *ast);
void freeNode(AST_NODE *p);

#endif
