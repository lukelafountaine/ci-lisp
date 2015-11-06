The following grammar expands the capability of ciLisp by adding support for user-provided and random data:

program ::= | program s_expr EOL
s_expr ::=
( cond s_expr s_expr s_expr )
| ( ( let let_list ) s_expr )
| symbol
| number
let_list ::= let_elem | let_list let_elem
let_elem ::= ( symbol s-expr ) | ( type symbol s-expr ) type ::= integer | real
symbol ::= letter+
letter ::= [a-zA-Z]
func ::= neg|abs|exp|sqrt|add|sub|mult|div|remainder|log|pow|max|min|exp2|cbrt|hypot|rand|read|print|equal|smaller|larger
number ::= [+|-]?digit+[.number+]
digit ::= [0-9]

The following is the summary of changes:

Added input functionality through a parameter-less function read. A symbol can be set to a value obtained in from the input as follows:

((let (a (read)) (b (read))) (print (add a b)))

Added a function that generates random numbers. The function does not take any parameters. It can be called as follows:

(cond (smaller (rand) 100) (add a 2) (sub a 2))

Expand the ciLisp compiler to handle compilation of ciLisp programs utilizing the new grammar. Then write an evaluator of abstract syntax trees that include the new features.

Write a program in ciLisp that forces the compiler and the evaluator to test the new functionality.

Submit the compiler's code (lex, yacc, C), the ciLisp program, and a log of your testing session that clearly shows the output of the compiler and the output of the evaluator.
