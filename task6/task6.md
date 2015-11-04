The following grammar further expands the capabilities of ciLisp by adding types for variables:

integers should provide precision equivalent to a long integer in C,
reals should have a precision equivalent to double in C.
program ::= | program s_expr EOL
s_expr ::= number
| symbol
| ( func s_expr )
| ( func s_expr s_expr )
| ( ( let let_list ) s_expr )
let_list ::= let_elem | let_list let_elem
let_elem ::= ( type symbol s_expr )
type ::= | integer | real
symbol ::= letter+
letter ::= [a-zA-Z]
func ::= neg|abs|exp|sqrt|add|sub|mult|div|remainder|log|pow|max|min|exp2|cbrt|hypot|print
number ::= [+|-]?digit+[.number+]
digit ::= [0-9]

The program should perform type checking as follows:

Variables must be initialized and declared with desired types before they can be used; for example:

((let (integer myVal 3)) (add myVal 1))

Embedded scopes inherit variable types; for example:

((let (int myA 3)) ((let (myA 5) (real myB 1.5)) (add myA myB)))

If an undeclared variable is used, the compiler should indicate an error:

"ERROR: undeclared variable <name> used"

where the name is the name of the variable.

Any attempt to re-declare a variable in the same scope should raise an error:

"ERROR: redeclaration of variable <name> attempted"

Variables should coerce as using the following rules:

integers should be assigned to reals by adding a zero fraction part; e.g., 1 -> 1.0, and
reals should be assigned to integer variables by rounding to the closest integer (down or up); e.g., 1.49 to 1 and 1.69 to 2.
he type of an expression should be integer if only expressions of type integer are used to compute it; otherwise, if should be real.
each time a casting is performed, the compiler should issue a warning:
"WARNING: incompatible type assignment for variable <name>"

Expand the ciLisp compiler to handle compilation of ciLisp programs utilizing the new grammar. Then write an evaluator of abstract syntax trees that include the new features.

Write a program in ciLisp that forces the compiler and the evaluator to test the new functionality.

Submit the compiler's code (lex, yacc, C), the ciLisp program, and a log of your testing session that clearly shows the output of the compiler and the output of the evaluator.
