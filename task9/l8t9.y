/*

s-expressions calculator

program ::= program s-expr

s-expr ::= number | symbol | (func) | ( func s-expr ) | ( func s-expr s-expr ) | ( cond s_expr s_expr s_expr ) | ( ( let let_list ) s_expr )

let_list ::= let_elem | let_list let_elem

let_elem ::= ( type symbol s_expr ) | ( symbol s_expr )

type ::= integer | real

symbol ::= letter+

letter ::= [a-zA-Z]

number ::= [ + | - ]? digit+ [ . digit+ ]

digit ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

*/

%{
#include "l8t9.h"
%}

%union
{
   double dval;
   char *sval;
   struct ast_node *astNode;
   struct symbol_ast_node* symNode;
};

%token <sval> FUNC
%token <sval> TYPE
%token <sval> SYMBOL
%token <dval> NUMBER
%token LPAREN RPAREN EOL QUIT LET COND

%type <astNode> s_expr
%type <symNode> let_elem
%type <symNode> let_list

%%

program:/* empty */ {
                       printf("> ");
                    }
        | program s_expr EOL
          {
          eval($2);
          //printf("\n%lf", eval($2)->value);
          freeNode($2);
          printf("\n> ");
          }
          ;

s_expr:
        NUMBER
        {
          //printf("NUMBER: %lf\n", $1);
          $$ = number($1);
        }

        | SYMBOL
        {
          //printf("SYMBOL %s\n", $1);
          $$ = symbol($1);
        }
        | LPAREN FUNC RPAREN
        {
          //printf("yacc: LPAREN FUNC s_expr RPAREN\n");
          $$ = function($2, 0, 0);
          //printf("%s(%lf)", $2, eval($3));
        }
        | LPAREN FUNC s_expr RPAREN
        {
          //printf("yacc: LPAREN FUNC s_expr RPAREN\n");
          $$ = function($2, $3, 0);
          //printf("%s(%lf)", $2, eval($3));
        }
        | LPAREN FUNC s_expr s_expr RPAREN
        {
          //printf("LPAREN FUNC s_expr s_expr RPAREN\n");
          //$$ = calc($2, $3, $4);
          $$ = function($2, $3, $4);
        }
        | LPAREN COND s_expr s_expr s_expr RPAREN
        {
          //printf("LPAREN FUNC s_expr s_expr RPAREN\n");
          //$$ = calc($2, $3, $4);
          $$ = condition($3, $4, $5);
        }
        | LPAREN LPAREN LET let_list RPAREN s_expr RPAREN
        {
          //printf("LPAREN LPAREN LET let_list RPAREN s_expr RPAREN\n");
          $$ = let($4, $6);
        }
        | QUIT
        {
          //printf("QUIT\n");
          exit(0);
        }

        | error
        {
          //printf("error\n");
          //printf("> ");
        }
        ;

let_list:
        let_elem
        {
          //printf("let_elem\n");
          $$ = let_list($1, NULL);
        }

        | let_list let_elem
        {
            //printf("let_list let_elem\n");
            $$ = let_list($2, $1);
        }
        ;

let_elem:
        LPAREN TYPE SYMBOL s_expr RPAREN
        {
          //printf("LPAREN %s SYMBOL s_expr RPAREN\n", $2);
          $$ = let_elem($2, $3, $4);
        }

        | LPAREN SYMBOL s_expr RPAREN
        {
          //printf("LPAREN SYMBOL s_expr RPAREN\n", $2);
          $$ = let_elem(NULL, $2, $3);
        }
        ;

%%
