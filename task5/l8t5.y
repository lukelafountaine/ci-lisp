/*

s-expressions calculator

program ::= program s-expr

s-expr ::= number | symbol | ( func s-expr ) | ( func s-expr s-expr ) | ( ( let let_list ) s_expr )

let_list ::= let_elem | let_list let_elem

let_elem ::= ( symbol s_expr )

symbol ::= letter+

letter ::= [a-zA-Z]

number ::= [ + | - ]? digit+ [ . digit+ ]

digit ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

*/

%{
#include "l8t5.h"
%}

%union
{
   double dval;
   char *sval;
   struct ast_node *astNode;
};

%token <sval> FUNC
%token <sval> SYMBOL
%token <dval> NUMBER
%token LPAREN RPAREN EOL QUIT LET

%type <astNode> s_expr
%type <astNode> let_elem

%%

program:/* empty */ {
                       printf("> ");
                    }
        | program s_expr EOL {
                              printf("yacc: program expr\n");
                              //printf("\n%f", eval($2));
                              //freeNode($2);
                              printf("\n> ");
                           }
        ;

s_expr:
        NUMBER {
                  printf("NUMBER%lf\n", $1);
                  //$$ = number($1);
               }

        | SYMBOL {
                  printf("SYMBOL");
                  //$$ = symbol($1)
        }
        | LPAREN FUNC s_expr RPAREN {
                                     printf("yacc: LPAREN FUNC expr RPAREN\n");
                                     //$$ = function($2, $3, 0);
                                     //printf("%s(%lf)", $2, eval($3));
                                  }
        | LPAREN FUNC s_expr s_expr RPAREN {
                                          printf("LPAREN FUNC expr expr RPAREN\n");
                                          // $$ = calc($2, $3, $4);
                                          //$$ = function($2, $3, $4);
                                       }
        | LPAREN LPAREN LET let_list RPAREN s_expr RPAREN {
                                          printf("LPAREN LPAREN LET let_list RPAREN s_expr RPAREN\n");
                                          //$$ = let($4, $6);
                                                      }
        | QUIT {
                  printf("QUIT\n");
                  exit(0);
               }

        | error {
                  printf("error\n");
                  //printf("> ");
                }
        ;

let_list:
        let_elem
        {
          printf("let_elem\n");
          //$$ = let_list($1, NULL);
        }

        | let_list let_elem {
            printf("let_list let_elem\n");
            //$$ = let_list($2, $1);
            }
        ;

let_elem:
        LPAREN SYMBOL s_expr RPAREN
        {
          printf("LPAREN SYMBOL s_expr RPAREN\n");
          //$$ = let_elem($2, $3);
        }
        ;
%%
