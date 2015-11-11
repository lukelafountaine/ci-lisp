# ciLisp
## An interpreter for CiLisp written in C

This project was for my COMP232 class - Programming Language Design. It uses flex and bison for the grammar and parsing aspects. The project begins basically as a calculator and then each task adds additional functionality. 

This languages uses s-epxressions which are a special notation for operations (functions), in which the operator name and the operands are enclosed in parentheses. For example, "1+2" is noted as:

`(add 1 2)`


Any function in ciLisp can be built in that way using the following grammar:
```
program ::= | program s_expr EOL
s_expr ::=
  ( cond s_expr s_expr s_expr )
  | ( ( let let_list ) s_expr )
  | ( func s_expr_list )
  | ( symbol ( s_expr_list ) )
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
```

Task 10 implements all of the functionality using static scoping.
