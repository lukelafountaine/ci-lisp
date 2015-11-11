/*

s_expressions calculator

program ::= | program s_expr EOL

s_expr ::=
  ( cond s_expr s_expr s_expr )
  | ( ( let let_list ) s_expr )
  | ( func s_expr_list )
  | ( symbol s_expr_list )
  | symbol
  | number
  | QUIT
  |

s_expr_list ::= | s_expr_list s_expr

let_list ::= let_elem | let_list let_elem

let_elem ::= ( symbol s_expr ) | ( type symbol s_expr ) | ( symbol ( arg_list ) s_expr )

type ::= integer | real

arg_list ::= symbol arg_list |

symbol ::= letter+

letter ::= [a-zA-Z]

number ::= [+|-]?digit+[.number+]

digit ::= [0-9]

*/

%{
#include "l8t10.h"
%}

%union
{
   double dval;
   char *sval;
   struct ast_node *astNode;
   struct symbol_ast_node* symNode;
   struct s_expr_list* argNode;
};

%token <sval> FUNC
%token <sval> TYPE
%token <sval> SYMBOL
%token <dval> NUMBER
%token LPAREN RPAREN EOL QUIT LET COND

%type <astNode> s_expr
%type <astNode> s_expr_list
%type <symNode> arg_list
%type <symNode> let_elem
%type <symNode> let_list

%%

program:/* empty */
        {
          printf("> ");
        }

        | program s_expr EOL
        {
          printf("\n%lf", eval($2)->value);
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
          $$ = symbol($1, NULL);
        }
        
        | LPAREN FUNC s_expr_list RPAREN
        {
          //printf("yacc: LPAREN %s s_expr_list RPAREN\n", $2);
          $$ = function($2, $3, 0);
          //printf("%s(%lf)", $2, eval($3));
        }

        | LPAREN SYMBOL LPAREN s_expr_list RPAREN RPAREN
        {
          //printf("calling a function\n");
          $$ = symbol($2, $4);
        }

        | LPAREN COND s_expr s_expr s_expr RPAREN
        {
          //printf("LPAREN COND s_expr s_expr s_expr RPAREN\n");
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

s_expr_list:
        {
          // do nothing
        }

        | s_expr_list s_expr
        {
          // for calling a function
          //printf("s_expr_list s_expr\n");
          $$ = s_expr_list($1, $2);
        }
        ;

// for defining the function
arg_list:
        {
          // do nothing
        }

        | SYMBOL arg_list
        {
          //printf("%s arg_list\n", $1);
          $$ = arg_list($1, $2);
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
        LPAREN SYMBOL s_expr RPAREN
        {
          //printf("LPAREN %s s_expr RPAREN\n", $2);
          $$ = let_elem(NULL, $2, NULL, $3);
        }

        |
        LPAREN TYPE SYMBOL s_expr RPAREN
        {
          //printf("LPAREN %s %s s_expr RPAREN\n", $2, $3);
          $$ = let_elem($2, $3, NULL, $4);
        }

        | LPAREN SYMBOL LPAREN arg_list RPAREN s_expr RPAREN
        {
          //printf("LPAREN %s LPAREN arg_list RPAREN s_expr RPAREN\n", $2);
          // defining a function so pass 'func' in as the type
          $$ = let_elem("func", $2, $4, $6);
        }
        ;
%%
