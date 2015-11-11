#ifndef __l8t10_h_
#define __l8t10_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "y.tab.h"

int yyparse(void);
int yylex(void);
void yyerror(char *);

typedef enum { INVALID=-1, INTEGER, REAL, FUNCTION } SYMBOL_TYPE;
typedef enum { NUM_TYPE, FUNC_TYPE, LET_TYPE, SYM, COND_TYPE } AST_NODE_TYPE;
typedef enum { NEG = 0, ABS, EXP, SQRT, EXP2, CBRT, HYPOT, ADD, SUB, MULT, DIV, MOD, LOG, POW, MAX, MIN, PRINT, EQUAL, SMALLER, LARGER, RAND, READ} FUNC_NAMES;

typedef struct scope_node
{
   struct symbol_ast_node* symbols;
   struct scope_node* parent;
} SCOPE_NODE;

typedef struct
{
    SYMBOL_TYPE type;
    double value;
} NUMBER_AST_NODE;

typedef struct
{
    struct ast_node *condition;
    struct ast_node *true_expr;
    struct ast_node *false_expr;
} COND_AST_NODE;

typedef struct function_ast_node
{
   char *name;
   struct ast_node *op1;
   struct ast_node *op2;
} FUNCTION_AST_NODE;

typedef struct s_expr_list
{
  struct ast_node* arg;
  struct s_expr_list* next;
}S_EXPR_LIST;

typedef struct symbol_ast_node
{
   char* name;
   SYMBOL_TYPE type;
   struct ast_node* value;
   struct symbol_ast_node* next;
   struct symbol_ast_node* args;
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
      COND_AST_NODE condition;
   } data;
   struct ast_node* next;
} AST_NODE;

// functions for creating ast_nodes
AST_NODE* number(double value);
AST_NODE* function(char *funcName, AST_NODE *op1, AST_NODE *op2);
AST_NODE* let(SYMBOL_AST_NODE *symbols, AST_NODE *s_expr);
SYMBOL_AST_NODE* let_list(SYMBOL_AST_NODE *symbol, SYMBOL_AST_NODE *let_list);
SYMBOL_AST_NODE* let_elem(char* type, char* symbol, SYMBOL_AST_NODE* arg_list, AST_NODE* s_expr);
AST_NODE *condition(AST_NODE* condition, AST_NODE* ifTrue, AST_NODE* ifFalse);
AST_NODE* symbol(char *name, AST_NODE* args);

// creating the arg_list associated with a function definition
SYMBOL_AST_NODE* arg_list(char* symbol, SYMBOL_AST_NODE* arg_list);
AST_NODE* s_expr_list(AST_NODE* s_expr_list, AST_NODE* s_expr);

// functions for doing stuff with the symbol table
double getSymbolValue(char* name);
void leaveScope();
void enterScope(SCOPE_NODE* newScope);

// functions for other stuff
NUMBER_AST_NODE* eval(AST_NODE *ast);
void freeNode(AST_NODE *p);

#endif
