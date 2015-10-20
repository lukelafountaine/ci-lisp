#include <math.h>
#include "l8t1.h"

int main(void)
{
  yyparse();
  return 0;
}

void yyerror(char *s)
{
  fprintf(stderr, "%s\n", s);
}

int resolveFunc(char *func)
{
  char *funcs[] = { "neg", "abs", "exp", "sqrt", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", "exp2", "cbrt", "hypot",""};

  int i = 0;
  while (funcs[i][0] !='\0')
  {
    if (!strcmp(funcs[i], func))
      return i;
    i++;
  }
  yyerror("invalid function"); // paranoic -- should never happen
  return -1;
}

double calc(char *func, double op1, double op2)
{
  double result = 0.0;
  int function = resolveFunc(func);

  if (function == NEG)
    result = -op1;
  else if (function == ABS)
    result = fabs(op1);
  else if (function == EXP)
    result = exp(op1);
  else if (function == SQRT)
    result = sqrt(op1);
  else if (function == ADD)
    result = op1 + op2;
  else if (function == SUB)
      result = op1 - op2;
  else if (function == MULT)
      result = op1 * op2;
  else if (function == DIV)
      result = op1 / op2;
  else if (function == MOD)
      result = remainder(op1, op2);
  else if (function == LOG)
  {
      if (op1 == 2)
        result = log2(op2);
      else if (op1 == 10)
        result = log10(op2);
      else
        yyerror("invalid log base");
  }
  else if (function == POW)
      result = pow(op1, op2);
  else if (function == MAX)
      result = fmax(op1, op2);
  else if (function == MIN)
      result = fmin(op1, op2);
  else if (function == EXP2)
      result = exp2(op1);
  else if (function == CBRT)
      result = cbrt(op1);
  else if (function == HYPOT)
      result = hypot(op1, op2);

  return result;
}
