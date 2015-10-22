The following grammar expands the ciLisp grammar that you used so far by adding a "let section" that allows one to declare symbols (Lisp jargon for variables). Please note that a symbol can be any number of small and capital letters:

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
func ::= neg|abs|exp|sqrt|add|sub|mult|div|remainder|log|pow|max|min|exp2|cbrt|hypot
number ::= [+|-]?digit+[.number+]
digit ::= [0-9]

Here are some sample expressions that use symbols:

(add ((let (abc 1)) (sub -3 abc)) 4)
(mult ((let (a 1) (b 2)) (add a b)) (sqrt 2))
(add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))

As you see, s-expressions are first class objects, so they can be assigned to variables.

Implement a bison-based parser of the language built over this grammar that creates a syntax tree and a symbol table of all identifiers. Assume static scoping. That means that there will be multiple symbol tables associated with the root of the expression within which they are defined. For example, using the following code sample, there would be a symbol table associated with add in the outer expression, and another associated with add in the inner expression:

((let (abc 1)) (sub ((let (abc 2) (de 3)) (add abc de)) abc))

Missing symbols, for example as in the following expression:

((let (abc 1)) (sub ((let (abc 2)) (add abc de)) abc))
should be treated as compilations errors.

Redefining a symbol in the same scope should generate a warning, but the last definition should be assumed.

You may use a linked list to keep the symbols. You will need to keep symbol names along with their values. Note that the value may be an s-expression, so you should just keep a pointer to the root of the corresponding abstract syntax tree (that obviously may be just one element, the root holding the value, in the extreme case).

You will need to add a node for symbol references to the nodes available for constructing abstract syntax trees of the programs.

Write a program in ciLisp that forces the compiler and the evaluator to test the new functionality (both positively and negatively). For example, the following expressions should compile and evaluate with no errors:

(add ((let (abcd 1)) (sub -3 abcd)) 4)
(mult ((let (a 1) (b 2)) (add a b)) (sqrt 2))
(add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))
((let (a ((let (c 3) (d 4)) (mult c d)))) (sqrt a))

Implement the evaluation function that executes this and similar programs. Note that the code will have to perform symbol table lookups. Furthermore, keep in mind that symbols may hold s-expressions rather than straight numerical values. Therefore, you may need to evaluate s-expressions that are bound to the symbols.

Submit the source code for lex, yacc, and c (*.c and *.h). As usually, attach ample test runs of your compiler.
