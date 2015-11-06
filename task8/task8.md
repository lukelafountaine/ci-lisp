The following grammar expands the capability of ciLisp by adding conditional expressions:

program ::= | program s_expr EOL
s_expr ::= number
| symbol
| ( func s_expr )
| ( func s_expr s_expr )
| ( cond s_expr s_expr s_expr )
| ( ( let let_list ) s_expr )
let_section ::= ( let let_list )
let_list ::= let_elem | let_list let_elem
let_elem ::= ( type symbol s_expr )
type ::= | integer | real
symbol ::= letter+
letter ::= [a-zA-Z]
func ::= neg|abs|exp|sqrt|add|sub|mult|div|remainder|log|pow|max|min|exp2|cbrt|hypot|print|equal|smaller|larger
number ::= [+|-]?digit+[.number+]
digit ::= [0-9]

The following is the summary of changes:

Added comparison functions equal, smaller, and larger. The functions return 1 if the condition holds, and 0 otherwise.

Added cond function that checks if the first s-expression is true (non-zero). If so, the the next expression is evaluated and returned. Otherwise, the third s_expression is evaluated and returned. For example:

(cond (smaller myA myB) (print myA) (print myB))

should print the value of myA if the expression (smaller myA myB) is true (non-zero); otherwise, the value of myB should be printed.
If the function is not one of the relationship functions (i.e., neither equal, smaller, nor larger), then the expression should be evaluated, and if it equals to zero, then the condition should be considered false; otherwise (any value other than zero), the condition should be considered true.

Expand the ciLisp compiler to handle compilation of ciLisp programs utilizing the new grammar. Then write an evaluator of abstract syntax trees that include the new features.

Write a program in ciLisp that forces the compiler and the evaluator to test the new functionality.

Submit the compiler's code (lex, yacc, C), the ciLisp program, and a log of your testing session that clearly shows the output of the compiler and the output of the evaluator.
