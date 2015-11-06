#include <math.h>
#include <string.h>
#include "l8t7.h"

SCOPE_NODE* currentScope;

int main(void)
{
  yyparse();
  return 0;
}

void yyerror(char *s)
{
  fprintf(stderr, "%s\n", s);
}

double getSymbolValue(char *name)
{
  // make a new node to iterate through the table
  SCOPE_NODE* currScope = currentScope;
  SYMBOL_AST_NODE* currentSymbol = currScope->symbols;

  double result = 0.0;
  int found = 0;

  // start with current scope and work up through parents
  while(currScope)
  {
    while (currentSymbol)
    {
      //check to see if this is the symbol were looking for
      if (!strcmp(currentSymbol->name, name)) {
        result = eval(currentSymbol->value);
        found = 1;
        break;
      }
      // otherwise keep on iterating
      currentSymbol = currentSymbol->next;
    }
    if (found) break;
    // move to the next scope and keep looking
    currScope = currScope->parent;
    if (currScope)
      currentSymbol = currScope->symbols;
  }
  // throw error if its not found
  if (!found)
    yyerror("ERROR: Undeclared variable <%s>!\n");

  return result;
}

void leaveScope()
{
  if(currentScope)
    currentScope = currentScope->parent;
}

void enterScope(SCOPE_NODE* newScope)
{
  newScope->parent = currentScope;
  currentScope = newScope;
}

// find out which function it is
int resolveFunc(char *func)
{
  char *funcs[] = { "neg", "abs", "exp", "sqrt", "exp2", "cbrt", "hypot", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", "print", ""};

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

// create a node for let
AST_NODE *let(SYMBOL_AST_NODE *symbols, AST_NODE *s_expr)
{
  AST_NODE *p;
  size_t nodeSize;

  // create a new scope node
  SCOPE_NODE *localScope = malloc(sizeof(SCOPE_NODE));
  localScope->symbols = symbols;

  // allocate space for the fixed size and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(LET_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  // put scope and s_expr into the let node and return it
  p->type = LET_TYPE;
  p->data.let.scope = localScope;
  p->data.let.s_expr = s_expr;

  return p;
}

// add the new symbol to the list and return it
SYMBOL_AST_NODE* let_list(SYMBOL_AST_NODE *symbol, SYMBOL_AST_NODE *let_list)
{
  // insert the new symbol into the let_list
  SYMBOL_AST_NODE* current = let_list;
  int found = 0;

  // check for duplicates
  while (current)
  {
    if (!strcmp(symbol->name, current->name))
    {
      printf("ERROR: redeclaration of variable <%s> attempted", symbol->name);
      current->value = symbol->value;
      found = 1;
      symbol = let_list;
      break;
    }
    // keep looking through the list
    current = current->next;
  }

  // otherwise add it to the front of the list
  if (!found)
    symbol->next = let_list;

  // return the list with the new symbol
  return symbol;
}

// create a new symbol and return it
SYMBOL_AST_NODE *let_elem(char* symbol, AST_NODE *s_expr)
{
  SYMBOL_AST_NODE *p;
  size_t nodeSize;

  // allocate space the symbol
  nodeSize = sizeof(SYMBOL_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->name = symbol;
  p->value = s_expr;
  return p;
}

// create a symbol node
AST_NODE *symbol(char* name)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed sie and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(SYMBOL_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = SYM;
  p->data.symbol.name = name;
  return p;
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
      case PRINT:
        printf
    }
  }
  // just evaluate the right side of each let
  else if (p->type == LET_TYPE)
  {
    //enter the new scope, evaluate, then leave
    enterScope(p->data.let.scope);
    result = eval(p->data.let.s_expr);
    leaveScope();
  }

  else if (p->type == SYM)
    result = getSymbolValue(p->data.symbol.name);

  return result;
}
