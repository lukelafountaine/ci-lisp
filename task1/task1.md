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
In this task, you are to complete the implementation of a bison-based interpreter of a language that allows for computation of copmplex s-expressions.

Please download l8t1.zip file that includes a starting implementation. Study the included yacc, lex, and C code, so you understand how all bits and pieces work. Note that the following part of the bison code allows you to specify types of some tokens. That is part of semantic analysis that we will study in a few weeks.

%union
{
double dval;
char *sval;
}

%token <sval> FUNC
%token <dval> NUMBER
%token LPAREN RPAREN EOL QUIT

%type <dval> s_expr
When you understand the code, move on to implement the missing functionality, so that every s-expression is interpreted while being analyzed. The following is a sample session:

$ ./l8t1build.sh
$ ./l8t1
> (add 1 2)
3.000000
> (add (exp (sub 3.5 (sqrt 15))) (log (remainder (pow 3 (sqrt 20)) 5.2)))
0.626430
> quit
Please note that Math library is included by default, but to use the functions we need to include <math.h>.

Also note that the value in the example assumes that logarithm with the base of 10 is used as implemented by log10(). If you use natural logarithm (as implemented by log()), then the result will be as follows:

$ ./l8t1
> (add (exp (sub 3.5 (sqrt 15))) (log (remainder (pow 3 (sqrt 20)) 5.2)))
0.545349
> quit
