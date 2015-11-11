The following grammar expands the capability of ciLisp by adding support for user-defined functions:

program ::= | program s_expr EOL
s_expr ::=
( cond s_expr s_expr s_expr )
| ( ( let let_list ) s_expr )
| ( func s-expr_list )
| ( symbol s-expr_list )
| symbol
| number
s-expr_list ::= | s-expr_list s-expr
let_list ::= let_elem | let_list let_elem
let_elem ::= ( symbol s-expr ) | ( type symbol s-expr ) | ( symbol ( arg_list ) s-expr )
type ::= integer | real
arg_list ::= symbol arg_list |
symbol ::= letter+
letter ::= [a-zA-Z]
func ::= neg|abs|exp|sqrt|add|sub|mult|div|remainder|log|pow|max|min|exp2|cbrt|hypot|rand|read|print|equal|smaller|larger
number ::= [+|-]?digit+[.number+]
digit ::= [0-9]

Function definitions have been added to the let section (to keep things simple). A function may have a number of formal arguments (symbols). The body of the function is an expression that may use formal parameters. For example:

((let (myFunc (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5) 2))

As shown, a function must be called with the number of actual parameters that comply with the arity of the definition. The value of the expression that is the body of the function is the value that the function returns.

Please note that a function with no parameters is just a symbol associated with an expression. For example:

((let (myPow () (pow 2.0 10.0))) (add (myPow) 3))

is same as:

((let (myPow (pow 2.0 10.0))) (add myPow 3))

Expand the ciLisp compiler to handle compilation of ciLisp programs utilizing the new grammar. Then write an evaluator of abstract syntax trees that include the new features.

Write a program in ciLisp that forces the compiler and the evaluator to test the new functionality.

Submit the compiler's code (lex, yacc, C), the ciLisp program, and a log of your testing session that clearly shows the output of the compiler and the output of the evaluator.
