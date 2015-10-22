l8t3.zip  file contains an implemention of a partial translator. It builds an abstract syntax tree, and then uses it to generate a new version of the source program. Study the code carefully, and then implement the missing functionality in function translate(). That function should translate any s-expression to a version that uses prefix notation for functions and infix notation for arithmetic operations. Don't forget to include the additional functions that you implemented in Task 2.

The following is a sample run that should be used as a guide:

$ ./l8t3
> (add (exp (sub 3.5 (sqrt 15))) (abs (remainder (pow 3 (sqrt 20)) 5.2)))
( exp( ( 3.500000 - sqrt( 15.000000 ) ) ) + abs( remainder( pow( 3.000000, sqrt( 20.000000 ) ), 5.200000 ) ) )
>
