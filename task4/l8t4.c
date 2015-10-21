#include <math.h>
#include "l8t4.h"

char *arithmeticOps = "+-*/";
int main(void)
{
  yyparse();
  return 0;
}

void yyerror(char *s)
{
  fprintf(stderr, "%s\n", s);
}

// find out which function it is
int resolveFunc(char *func)
{
  char *funcs[] = { "neg", "abs", "exp", "sqrt", "exp2", "cbrt", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", ""};

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

// create a node for a number
AST_NODE *number(double value)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed sie and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(NUMBER_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = NUM_TYPE;
  p->data.number.value = value;

  return p;
}

// create a node for a function
AST_NODE *function(char *funcName, AST_NODE *op1, AST_NODE *op2)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed sie and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(FUNCTION_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = FUNC_TYPE;
  p->data.function.name = funcName;
  p->data.function.op1 = op1;
  p->data.function.op2 = op2;

  return p;
}

// free a node
void freeNode(AST_NODE *p)
{
  if (!p)
    return;

  if (p->type == FUNC_TYPE)
  {
    free(p->data.function.name);
    freeNode(p->data.function.op1);
    freeNode(p->data.function.op2);
  }

  free (p);
}

double eval(AST_NODE *p)
{
  double result = 0.0;
  if (!p)
    return result;

  else if (p->type == NUM_TYPE)
    result = p->data.number.value;

  else if (p->type == FUNC_TYPE)
  {
    int funcType = resolveFunc(p->data.function.name);
    switch (funcType)
    {
      //unary instructions
      case NEG:
        result = -eval(p->data.function.op1); break;
      case ABS:
        result = fabs(eval(p->data.function.op1)); break;
      case EXP:
        result = exp(eval(p->data.function.op1)); break;
      case SQRT:
        result = sqrt(eval(p->data.function.op1)); break;
      case EXP2:
        result = exp2(eval(p->data.function.op1)); break;
      case CBRT:
        result = cbrt(eval(p->data.function.op1)); break;

      //arithmetic operations
      case ADD:
        result = eval(p->data.function.op1) + eval(p->data.function.op2); break;
      case SUB:
        result = eval(p->data.function.op1) - eval(p->data.function.op2); break;
      case MULT:
        result = eval(p->data.function.op1) * eval(p->data.function.op2); break;
      case DIV:
        result = eval(p->data.function.op1) / eval(p->data.function.op2); break;

      // other binary operations
      case MOD:
        result = remainder(eval(p->data.function.op1), eval(p->data.function.op2)); break;
      case LOG:
        if (eval(p->data.function.op1) == 2.0)
          result = log2(eval(p->data.function.op2));
        else if (eval(p->data.function.op1) == 10.0)
          result = log10(eval(p->data.function.op2));
        else
          yyerror("invalid log base");
      break;
      case POW:
        result = pow(eval(p->data.function.op1), eval(p->data.function.op2)); break;
      case MAX:
        result = fmax(eval(p->data.function.op1), eval(p->data.function.op2)); break;
      case MIN:
        result = fmin(eval(p->data.function.op1), eval(p->data.function.op2)); break;
      case HYPOT:
        result = hypot(eval(p->data.function.op1), eval(p->data.function.op2)); break;
    }
  }
  return result;
}
