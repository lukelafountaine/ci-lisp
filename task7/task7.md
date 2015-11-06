The following grammar expands the capabilities of ciLisp by adding print function.

program ::= | program s_expr EOL
s_expr ::= number
| symbol
| ( func s_expr )
| ( func s_expr s_expr )
| ( ( let let_list ) s_expr )
let_list ::= let_elem | let_list let_elem
let_elem ::= ( symbol s_expr )
symbol ::= letter+
letter ::= [a-zA-Z]
func ::= neg|abs|exp|sqrt|add|sub|mult|div|remainder|log|pow|max|min|exp2|cbrt|hypot|print
number ::= [+|-]?digit+[.number+]
digit ::= [0-9]

The following expression:

(print s_expr)

should print the value of the s_expression in the format depending on the type of the expression.(integer or real). Reals should print with double precision with two spaces after the dot (even if there are only zeros to the right of the dot). The width of the print field should be minimized; i.e., values should use a tight format (no padding on the left).

Expand the ciLisp compiler to handle compilation of ciLisp programs utilizing the news functions. Then write an evaluator of abstract syntax trees that include the new features.

Write a program in ciLisp that forces the compiler and the evaluator to test the new functionality.

Submit the compiler's code (lex, yacc, C), the ciLisp program, and a log of your testing session that clearly shows the output of the compiler and the output of the evaluator.
