# ciLisp
An interpreter for CiLisp written in C

In this multi-task project that will span 4 labs, you will implement a compiler for ciLisp, a simple language based on s-expressions. We will call it ciLisp (Channel Islands Lisp), since it is similar to Lisp programming langauge that you will have a chance to use in a later segment of this course.

This is a challanging project that requires your focus throughout the 4-week span. The later parts are much more challanging than the initial tasks, so to stay on tracks and being able to wrap things up you should follow the following timeline as closely as possible:

Lab 8: Task 1, Task 2, Task 3, Task 4
Lab 9: Task 5
Lab 10: Task 6, Task 7, Task 8, Task 9
Lab 11: Task 10

Using a tool called Gantt Chart is useful to manage such long-term project. Have a look at the following example (https://www.tomsplanner.com/public/cilisp?) that I created for this project. Use the same password as for the lecture notes.

To implement the compiler, you will use flex (lex) and bison (yacc) that you learned in one the previous lab. The implementation will span several stages; each stage will add a feature to the compiler. If you complete all stages (i.e., implement all specified features) then you will get maximum points for the whole project; otherwise, you will earn partial credit according to the number of features that you implemented.

An s-epxression is a special notation of operations (functions), in which the operator name and the operands are enclosed in parentheses. For example, "1+2" is noted as:

(add 1 2)
Any function can be used in that way, and more complex can be built using the following grammar:

s-expr ::= number | ( func s-expr ) | ( func s-expr s-expr )
number ::= [ + | - ] digit+ [ . digit+ ] 
digit ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

For example:

(add (exp (sub 3.5 (sqrt 15))) (log (remainder (pow 3 (sqrt 20)) 5.2)))

You might be more familiar with another equivalent notation:

add(exp(sub(3.5, sqrt(15))), log(remainder(pow(3, sqrt(20)), 5.2)))

...or, even more familiar:

exp(3.5 - sqrt(15)) + log(remainder(pow(3, sqrt(20)), 5.2))
