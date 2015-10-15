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
  char *funcs[] = { "neg", "abs", "exp", "sqrt", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", ""};

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
  
  if (!strcmp(func, "neg"))
    result = -op1;
  else if (!strcmp(func, "abs"))
    result = fabs(op1);
  else if (!strcmp(func, "exp"))
    result = exp(op1);
  else if (!strcmp(func, "sqrt"))
    result = sqrt(op1);
  else if (!strcmp(func, "add"))
    result = op1 + op2;
  else if (!strcmp(func, "sub"))
    result = op1 - op2;
  else if (!strcmp(func, "mult"))
    result = op1 * op2;
  else if (!strcmp(func, "div"))
    result = op1 / op2;
  else if (!strcmp(func, "remainder"))
    result = remainder(op1, op2);
  else if (!strcmp(func, "log"))
    result = log(op1);
  else if (!strcmp(func, "pow"))
    result = pow(op1, op2);
  else if (!strcmp(func, "max"))
    result = fmax(op1, op2);
  else if (!strcmp(func, "min"))
    result = fmin(op1, op2);
  return result;
}
